/*****************************************************************************
  FileName：    C_RecvCanThread.cpp
  Author:       zhulu
  Version:      1.0
  Date:         2015/12/8 13:28:57
  Description:  接收can数据的类
  History:
                <author>    <time>    <version>    <desc>
                 zhulu    2015.12.08    V1.0       create
*****************************************************************************/
#include "C_RecvCanThread.h"
#include <QDebug>
#include <QTime>
#include <QCursor>
#include <QFile>

#define C_PAD_CALIBRATE_FILE         "/etc/pointercal"  //保存校验后的信息,以后根据该信息计算出坐标

/*********************************************************
Function:       C_RecvCanThread::C_RecvCanThread
Description:    C_RecvCanThread 类的构造函数
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
    Description:    根据/dev/input/uniput映射event设备
    Param:          none
    Return:         成功返回event设备描述符
                    失败返回-1
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
    Description:    读取can数据
    Param:          none
    Return:         成功返回can设备描述符
                    失败返回-1
    Other:          none
***********************************************************************/
int C_RecvCanThread::PadOpenCanDevice(void)
{
    struct sockaddr_can addr;
    struct ifreq ifr;

    /*建立套接字，设置为原始套接字，原始CAN协议 */
    if((mCanFd = ::socket(PF_CAN,SOCK_RAW,CAN_RAW)) < 0 )
    {
        qDebug("PadOpenCanDevice socket error");
        return -1;
    }

    /*以下是对CAN接口进行初始化，如设置CAN接口名，即当我们用ifconfig命令时显示的名字 */
    strcpy(ifr.ifr_name,"can0");

    if(0 > ::ioctl(mCanFd, SIOCGIFINDEX, &ifr))
    {
        qDebug("PadOpenCanDevice SIOCGIFINDEX error");
        close(mCanFd);
        return -1;
    }

    /*设置CAN协议 */
    addr.can_family = AF_CAN;
    addr.can_ifindex = 0;

    /*将刚生成的套接字与网络地址进行绑定*/
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
    //显示校验的界面
    QFile file(C_PAD_CALIBRATE_FILE);
    if(file.open(QIODevice::ReadOnly))
    {
        //打开成功,获取校验信息
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
Description:    键盘事件接收线程
Param:          none
Return:         none
Other:          none
*********************************************************/
void C_RecvCanThread::run()
{
    fd_set readfds;
    Uint8  recvCanMsgBuf[256];	//保存接收到的CAN数据
    Uint16 recvCanMsgLen;		//保存接收到的CAN数据长度
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
            /*读取can数据*/
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
    Description:    读取can数据
    Param:          inCanFd         设备描述符
                    pOutMsg	      	保存接收的can数据内容
                    pOutRdLenLen    保存接收的can数据长度
    Return:         返回0表示正常接收完成
                    返回-1表示接收错误
                    返回-2表示接收消息头错误
    Other:          none
***********************************************************************/
int C_RecvCanThread::PadRecvCanMsg(int inCanFd,Uint8 *pOutMsg,Uint16 *pOutRdLen)
{
    struct can_frame frame;
    struct sockaddr_can addr;
    socklen_t addrLen = sizeof(addr);
    S_HteMsgHead   *psMsgHead = (S_HteMsgHead*)pOutMsg;
    int headLen = 0;

    /*开始读取数据-消息头需要3帧才读完,frame.id:can的ID,frame.can_dlc:can数据长度,frame.data:can数据内容*/
    *pOutRdLen = 0;

    /*读取消息头的第一帧*/
    if (0 > ::recvfrom(inCanFd, &frame, sizeof(struct can_frame), 0, (struct sockaddr *)&addr, &addrLen))
    {
        return -1;
    }

    /*判断是否是第一帧数据*/
    if(frame.data[0] != HTE_MSG_HEAD0 || frame.data[1] != HTE_MSG_HEAD1)
    {
        return -2;
    }
    memcpy(&pOutMsg[*pOutRdLen],frame.data,frame.can_dlc);
    *pOutRdLen += frame.can_dlc;

    /*读取消息头的第二帧*/
    if (0 > ::recvfrom(inCanFd, &frame, sizeof(struct can_frame), 0, (struct sockaddr *)&addr, &addrLen))
    {
        return -1;
    }

    memcpy(&pOutMsg[*pOutRdLen],frame.data,frame.can_dlc);
    *pOutRdLen += frame.can_dlc;

    /*读取消息头的第三帧*/
    if (0 > ::recvfrom(inCanFd, &frame, sizeof(struct can_frame), 0, (struct sockaddr *)&addr, &addrLen))
    {
        return -1;
    }

    memcpy(&pOutMsg[*pOutRdLen],frame.data,frame.can_dlc);
    *pOutRdLen += frame.can_dlc;

    headLen = (psMsgHead->msgLen[0] << 8) | psMsgHead->msgLen[1];
    //根据消息头的里面的长度,读取剩余的帧数
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
Description:    非校准过程中,处理can数据命令
Param:          pInMsg	      接收到的can数据
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
            /*触摸屏按下*/
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
            /*触摸屏释放*/
            PadCanTouchClick(BTN_LEFT,0x00);
        }
        break;
    case PAD_CAN_CMD_TOUCH_XY:
        {
            /*触摸坐标信息*/
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
Description:    向键盘鼠标驱动写入按键事件
Param:          inCode      请参考input.h的input_evet结构体的code定义
                inValue     0x01-按下,0x00-释放
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

    //同步数据
    PadCanTouchSync();
}

/***********************************************************************
Function:       C_RecvCanThread::PadCanTouchRelXY
Description:    向键盘鼠标驱动写入x,y坐标偏移量
Param:          inPointX	      X坐标
                inPointY	      Y坐标
Return:         none
Other:          write写入的是偏移量
***********************************************************************/
void C_RecvCanThread::PadCanTouchRelXY(int inPointX,int inPointY,int inPreX,int inPreY)
{
    struct input_event event;
    int relX = (inPointX - inPreX)/2;  //这里除以2的原因不清楚,调查中
    int relY = (inPointY - inPreY)/2;

    //写入X偏移量
    event.type = EV_REL;
    event.code = REL_X;
    event.value = relX;
    gettimeofday(&event.time, 0);

    if(0 <= ::write(mUinputFd, &event, sizeof(event)))
    {
        //mPreTouchX = inPointX;
    }

    //写入Y偏移量
    event.code = REL_Y;
    event.value = relY;
    gettimeofday(&event.time, 0);

    if(0 <= ::write(mUinputFd, &event, sizeof(event)))
    {
        //mPreTouchY = inPointY;
    }

    //同步数据
    PadCanTouchSync();
}

/***********************************************************************
Function:       C_RecvCanThread::PadCanTouchRelXY
Description:    向键盘鼠标驱动写入x,y坐标
Param:          inPointX	      X坐标
                inPointY	      Y坐标
Return:         none
Other:          write写入的是绝对坐标
***********************************************************************/
void C_RecvCanThread::PadCanTouchAbsXY(int inPointX,int inPointY)
{
    struct input_event event;

    //写入X偏移量
    event.type = EV_ABS;
    event.code = ABS_X;
    event.value = inPointX;
    gettimeofday(&event.time, 0);

    if(0 <= ::write(mUinputFd, &event, sizeof(event)))
    {
        //mPreTouchX = inPointX;
    }

    //写入Y偏移量
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

    //同步数据
    PadCanTouchSync();
}

/***********************************************************************
Function:       C_RecvCanThread::PadCanTouchSync
Description:    同步键盘鼠标驱动值
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

