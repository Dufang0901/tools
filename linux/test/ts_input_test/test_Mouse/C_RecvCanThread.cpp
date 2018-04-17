/*****************************************************************************
  FileName��    C_RecvCanThread.cpp
  Author:       zhulu
  Version:      1.0
  Date:         2015/12/8 13:28:57
  Description:  ����can���ݵ���
  History:
                <author>    <time>    <version>    <desc>
                 zhulu    2015.12.08    V1.0       create
*****************************************************************************/
#include "C_RecvCanThread.h"
#include <QDebug>
#include <QTime>
#include <QCursor>
#include <QFile>

#define C_PAD_CALIBRATE_FILE         "/etc/pointercal"  //����У������Ϣ,�Ժ���ݸ���Ϣ���������

/*********************************************************
Function:       C_RecvCanThread::C_RecvCanThread
Description:    C_RecvCanThread ��Ĺ��캯��
Param:          parent
Return:         none
Other:          none
*********************************************************/
C_RecvCanThread::C_RecvCanThread(QObject *parent) :
    QThread(parent)
{

}

/***********************************************************************
    Function:       C_RecvCanThread::PadSetupUniputDevice
    Description:    ����/dev/input/uniputӳ��event�豸
    Param:          none
    Return:         �ɹ�����event�豸������
                    ʧ�ܷ���-1
    Other:          none
***********************************************************************/
int C_RecvCanThread::PadSetupUniputDevice()
{
    struct uinput_user_dev uinp; // uInput device structure
    if((mUinputFd = ::open("/dev/input/uinput", O_WRONLY | O_NDELAY)) < 0)
    {
        qDebug("PadSetupUniputDevice open error");
        return -1;
    }

     memset(&uinp,0,sizeof(uinp)); // Intialize the uInput device to NULL
     strncpy(uinp.name, "padEvent", UINPUT_MAX_NAME_SIZE);
     uinp.id.version = 4;
     uinp.id.bustype = BUS_USB;

     // Setup the uinput device
     ::ioctl(mUinputFd, UI_SET_EVBIT, EV_KEY);
     ::ioctl(mUinputFd, UI_SET_EVBIT, EV_REL);
     ::ioctl(mUinputFd, UI_SET_RELBIT, REL_X);
     ::ioctl(mUinputFd, UI_SET_RELBIT, REL_Y);
     ::ioctl(mUinputFd, UI_SET_KEYBIT, BTN_LEFT);
     ::ioctl(mUinputFd, UI_SET_KEYBIT, BTN_MIDDLE);
     ::ioctl(mUinputFd, UI_SET_KEYBIT, BTN_RIGHT);
     ::ioctl(mUinputFd, UI_SET_KEYBIT, BTN_TOUCH);


     ::ioctl(mUinputFd, UI_SET_EVBIT, EV_ABS);
     ::ioctl(mUinputFd, UI_SET_ABSBIT, ABS_X);
     ::ioctl(mUinputFd, UI_SET_ABSBIT, ABS_Y);
     ::ioctl(mUinputFd, UI_SET_ABSBIT, ABS_PRESSURE);


     /* Create input device into input sub-system */
     ::write(mUinputFd, &uinp, sizeof(uinp));
     if(0 > ::ioctl(mUinputFd, UI_DEV_CREATE))
     {
        qDebug("PadSetupUniputDevice UI_DEV_CREATE error");
        ::close(mUinputFd);
        return -1;
     }

     return mUinputFd;
}

/***********************************************************************
    Function:       C_RecvCanThread::OpenCanDevice
    Description:    ��ȡcan����
    Param:          none
    Return:         �ɹ�����can�豸������
                    ʧ�ܷ���-1
    Other:          none
***********************************************************************/
int C_RecvCanThread::PadOpenCanDevice(void)
{
    struct sockaddr_can addr;
    struct ifreq ifr;

    /*�����׽��֣�����Ϊԭʼ�׽��֣�ԭʼCANЭ�� */
    if((mCanFd = ::socket(PF_CAN,SOCK_RAW,CAN_RAW)) < 0 )
    {
        qDebug("PadOpenCanDevice socket error");
        return -1;
    }

    /*�����Ƕ�CAN�ӿڽ��г�ʼ����������CAN�ӿ���������������ifconfig����ʱ��ʾ������ */
    strcpy(ifr.ifr_name,"can0");

    if(0 > ::ioctl(mCanFd, SIOCGIFINDEX, &ifr))
    {
        qDebug("PadOpenCanDevice SIOCGIFINDEX error");
        close(mCanFd);
        return -1;
    }

    /*����CANЭ�� */
    addr.can_family = AF_CAN;
    addr.can_ifindex = 0;

    /*�������ɵ��׽����������ַ���а�*/
    if(0 > ::bind(mCanFd, (struct sockaddr*)&addr, sizeof(addr)))
    {
        qDebug("PadOpenCanDevice bind error");
        close(mCanFd);
        return -1;
    }
    return mCanFd;
}

void C_RecvCanThread::PadInitCalibrate()
{
    //��ʾУ��Ľ���
    QFile file(C_PAD_CALIBRATE_FILE);
    if(file.open(QIODevice::ReadOnly))
    {
        //�򿪳ɹ�,��ȡУ����Ϣ
        char tempBuf[128];
        file.read(tempBuf,128);
        ::sscanf(tempBuf,"%f,%f,%f,%f",&mConstX,&mXiShuX,&mConstY,&mXiShuY);
        file.close();
    }
    else
    {
        mConstX = 0;
        mXiShuX = 0;
        mConstY = 0;
        mXiShuY = 0;
    }
}

/*********************************************************
Function:       C_RecvCanThread::run
Description:    �����¼������߳�
Param:          none
Return:         none
Other:          none
*********************************************************/
void C_RecvCanThread::run()
{
    fd_set readfds;
    Uint8  recvCanMsgBuf[256];	//������յ���CAN����
    Uint16 recvCanMsgLen;		//������յ���CAN���ݳ���
    S_HteMsgHead   *psMsgHead = (S_HteMsgHead*)recvCanMsgBuf;
    Uint16    canCrc;
    Uint16    canCrcRd;

#if 1
    if((mUinputFd = open("/dev/input/event0", O_RDWR)) < 0)
    {
        qDebug("C_RecvCanThread exit -  setup uniput device error");
        return;
    }
#else
    if(-1 == PadSetupUniputDevice())
    {
        qDebug("C_RecvCanThread exit -  setup uniput device error");
        return;
    }
#endif
    if(-1 == PadOpenCanDevice())
    {
        qDebug("C_RecvCanThread exit -  open can device error");
        ::close(mUinputFd);
        return;
    }
    PadInitCalibrate();

    QCursor::setPos(0,0);
    PadCanTouchRelXY(0,0,0,0);

    while(1)
    {
        FD_ZERO(&readfds);
        FD_SET(mCanFd, &readfds);
        int res = ::select(mCanFd + 1,&readfds,NULL, NULL, NULL);
        if(res <= 0)
        {
            continue;
        }

        if(FD_ISSET(mCanFd,&readfds))
        {
            /*��ȡcan����*/
            if(0 == PadRecvCanMsg(mCanFd,recvCanMsgBuf,&recvCanMsgLen))
            {
                canCrc = (psMsgHead->crc[0] << 8) | psMsgHead->crc[1];
                psMsgHead->crc[0] = 0;
                psMsgHead->crc[1] = 0;
                canCrcRd = HteMsgCrc(recvCanMsgBuf,recvCanMsgLen);
                if(canCrc == canCrcRd && psMsgHead->srcDevType == HTE_MSG_DEV_PAD2)
                {
                    QString temp;
                    QString tempStr = "";
                    for(int i=0;i<recvCanMsgLen;i++)
                    {
                        temp.sprintf("%02X ",recvCanMsgBuf[i]);
                        tempStr += temp;
                    }
                    qDebug()<<tempStr;


                    PadRecvCanMsgPro(recvCanMsgBuf);
                }
            }

        }

    }
    close(mCanFd);
    qDebug("C_RecvCanThread quit");

}

/***********************************************************************
    Function:       C_RecvCanThread::PadRecvCanMsg
    Description:    ��ȡcan����
    Param:          inCanFd         �豸������
                    pOutMsg	      	������յ�can��������
                    pOutRdLenLen    ������յ�can���ݳ���
    Return:         ����0��ʾ�����������
                    ����-1��ʾ���մ���
                    ����-2��ʾ������Ϣͷ����
    Other:          none
***********************************************************************/
int C_RecvCanThread::PadRecvCanMsg(int inCanFd,Uint8 *pOutMsg,Uint16 *pOutRdLen)
{
    struct can_frame frame;
    struct sockaddr_can addr;
    socklen_t addrLen = sizeof(addr);
    S_HteMsgHead   *psMsgHead = (S_HteMsgHead*)pOutMsg;
    int headLen = 0;

    /*��ʼ��ȡ����-��Ϣͷ��Ҫ3֡�Ŷ���,frame.id:can��ID,frame.can_dlc:can���ݳ���,frame.data:can��������*/
    *pOutRdLen = 0;

    /*��ȡ��Ϣͷ�ĵ�һ֡*/
    if (0 > ::recvfrom(inCanFd, &frame, sizeof(struct can_frame), 0, (struct sockaddr *)&addr, &addrLen))
    {
        return -1;
    }

    /*�ж��Ƿ��ǵ�һ֡����*/
    if(frame.data[0] != HTE_MSG_HEAD0 || frame.data[1] != HTE_MSG_HEAD1)
    {
        return -2;
    }
    memcpy(&pOutMsg[*pOutRdLen],frame.data,frame.can_dlc);
    *pOutRdLen += frame.can_dlc;

    /*��ȡ��Ϣͷ�ĵڶ�֡*/
    if (0 > ::recvfrom(inCanFd, &frame, sizeof(struct can_frame), 0, (struct sockaddr *)&addr, &addrLen))
    {
        return -1;
    }

    memcpy(&pOutMsg[*pOutRdLen],frame.data,frame.can_dlc);
    *pOutRdLen += frame.can_dlc;

    /*��ȡ��Ϣͷ�ĵ���֡*/
    if (0 > ::recvfrom(inCanFd, &frame, sizeof(struct can_frame), 0, (struct sockaddr *)&addr, &addrLen))
    {
        return -1;
    }

    memcpy(&pOutMsg[*pOutRdLen],frame.data,frame.can_dlc);
    *pOutRdLen += frame.can_dlc;

    headLen = (psMsgHead->msgLen[0] << 8) | psMsgHead->msgLen[1];
    //������Ϣͷ������ĳ���,��ȡʣ���֡��
    while(*pOutRdLen < headLen)
    {
        if (0 > ::recvfrom(inCanFd, &frame, sizeof(struct can_frame), 0, (struct sockaddr *)&addr, &addrLen))
        {
            return -1;
        }
        memcpy(&pOutMsg[*pOutRdLen],frame.data,frame.can_dlc);
        *pOutRdLen += frame.can_dlc;
    }

    return 0;
}

/***********************************************************************
Function:       C_RecvCanThread::PadRecvCanMsgPro
Description:    ��У׼������,����can��������
Param:          pInMsg	      ���յ���can����
Return:         none
Other:          none
***********************************************************************/
void C_RecvCanThread::PadRecvCanMsgPro(Uint8 *pInMsg)
{
    S_HteMsgHead   *psMsgHead = (S_HteMsgHead*)pInMsg;
    switch(psMsgHead->cmd)
    {
    case PAD_CAN_CMD_TOUCH_PRESS:
        {
            /*����������*/
            S_PadCanTouchXY sTouchXY;
            Uint8 *tempXY = (Uint8 *)(psMsgHead + 1);
            sTouchXY.valueX = (tempXY[0] << 8) | tempXY[1];
            sTouchXY.valueY = (tempXY[2] << 8) | tempXY[3];
            int x = sTouchXY.valueX*mXiShuX + mConstX;
            int y = sTouchXY.valueY*mXiShuY + mConstY;
            PadCanTouchRelXY(x,y,QCursor::pos().x(),QCursor::pos().y());

            PadCanTouchClick(BTN_LEFT,0x01);
        }
        break;
    case PAD_CAN_CMD_TOUCH_RELEASE:
        {
            /*�������ͷ�*/
            PadCanTouchClick(BTN_LEFT,0x00);
        }
        break;
    case PAD_CAN_CMD_TOUCH_XY:
        {
            /*����������Ϣ*/
            S_PadCanTouchXY sTouchXY;
            Uint8 *tempXY = (Uint8 *)(psMsgHead + 1);
            sTouchXY.valueX = (tempXY[0] << 8) | tempXY[1];
            sTouchXY.valueY = (tempXY[2] << 8) | tempXY[3];
            int x = sTouchXY.valueX*mXiShuX + mConstX;
            int y = sTouchXY.valueY*mXiShuY + mConstY;
            PadCanTouchRelXY(x,y,QCursor::pos().x(),QCursor::pos().y());

        }
        break;
    default:
        break;
    }
}

/***********************************************************************
Function:       C_RecvCanThread::PadCanTouchClick
Description:    ������������д�밴���¼�
Param:          inCode      ��ο�input.h��input_evet�ṹ���code����
                inValue     0x01-����,0x00-�ͷ�
Return:         none
Other:          none
***********************************************************************/
void C_RecvCanThread::PadCanTouchClick(int inCode,int inValue)
{
    struct input_event event;
    event.type  = EV_KEY;
    event.code  = inCode;
    event.value = inValue;
    gettimeofday(&event.time, 0);

    if(0 > ::write(mUinputFd, &event, sizeof(event)))
    {
        qDebug("C_RecvCanThread::PadCanTouchClick error");
        return;
    }

    //ͬ������
    PadCanTouchSync();
}

/***********************************************************************
Function:       C_RecvCanThread::PadCanTouchRelXY
Description:    ������������д��x,y����ƫ����
Param:          inPointX	      X����
                inPointY	      Y����
Return:         none
Other:          writeд�����ƫ����
***********************************************************************/
void C_RecvCanThread::PadCanTouchRelXY(int inPointX,int inPointY,int inPreX,int inPreY)
{
    struct input_event event;
    int relX = (inPointX - inPreX)/2;  //�������2��ԭ�����,������
    int relY = (inPointY - inPreY)/2;

    //д��Xƫ����
    event.type = EV_REL;
    event.code = REL_X;
    event.value = relX;
    gettimeofday(&event.time, 0);

    if(0 <= ::write(mUinputFd, &event, sizeof(event)))
    {
        //mPreTouchX = inPointX;
    }

    //д��Yƫ����
    event.code = REL_Y;
    event.value = relY;
    gettimeofday(&event.time, 0);

    if(0 <= ::write(mUinputFd, &event, sizeof(event)))
    {
        //mPreTouchY = inPointY;
    }

    //ͬ������
    PadCanTouchSync();
}

/***********************************************************************
Function:       C_RecvCanThread::PadCanTouchRelXY
Description:    ������������д��x,y����
Param:          inPointX	      X����
                inPointY	      Y����
Return:         none
Other:          writeд����Ǿ�������
***********************************************************************/
void C_RecvCanThread::PadCanTouchAbsXY(int inPointX,int inPointY)
{
    struct input_event event;

    //д��Xƫ����
    event.type = EV_ABS;
    event.code = ABS_X;
    event.value = inPointX;
    gettimeofday(&event.time, 0);

    if(0 <= ::write(mUinputFd, &event, sizeof(event)))
    {
        //mPreTouchX = inPointX;
    }

    //д��Yƫ����
    event.code = ABS_Y;
    event.value = inPointY;
    gettimeofday(&event.time, 0);

    if(0 <= ::write(mUinputFd, &event, sizeof(event)))
    {
        //mPreTouchY = inPointY;
    }
    event.code = ABS_PRESSURE;
    event.value = 1;
    gettimeofday(&event.time, 0);
    write(mUinputFd, &event, sizeof(event));

    event.code = ABS_PRESSURE;
    event.value = 0;
    gettimeofday(&event.time, 0);
    write(mUinputFd, &event, sizeof(event));

    //ͬ������
    PadCanTouchSync();
}

/***********************************************************************
Function:       C_RecvCanThread::PadCanTouchSync
Description:    ͬ�������������ֵ
Param:          none
Return:         none
Other:          none
***********************************************************************/
void C_RecvCanThread::PadCanTouchSync()
{
    struct input_event event;
    event.type  = EV_SYN;
    event.code  = SYN_REPORT;
    event.value = 0;
    gettimeofday(&event.time, 0);

    if(0 > ::write(mUinputFd, &event, sizeof(event)))
    {
        qDebug("C_RecvCanThread::PadCanTouchSync error");
    }
}

