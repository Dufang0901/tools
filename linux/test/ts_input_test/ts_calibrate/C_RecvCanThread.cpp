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

    if(-1 == PadOpenCanDevice())
    {
        qDebug("C_RecvCanThread exit -  open can device error");
        return;
    }
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
                    //У׼������
                    if(PAD_CAN_CMD_TOUCH_PRESS == psMsgHead->cmd)
                    {
                        /*����������*/
                        Uint8 *tempXY = (Uint8 *)(psMsgHead + 1);
                        Uint32 touchX = (tempXY[0] << 8) | tempXY[1];
                        Uint32 touchY = (tempXY[2] << 8) | tempXY[3];
                        emit signalClickPointF(touchX,touchY);
                    }
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
