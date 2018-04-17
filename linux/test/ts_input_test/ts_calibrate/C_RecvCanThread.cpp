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
            /*读取can数据*/
            if(0 == PadRecvCanMsg(mCanFd,recvCanMsgBuf,&recvCanMsgLen))
            {
                canCrc = (psMsgHead->crc[0] << 8) | psMsgHead->crc[1];
                psMsgHead->crc[0] = 0;
                psMsgHead->crc[1] = 0;
                canCrcRd = HteMsgCrc(recvCanMsgBuf,recvCanMsgLen);
                if(canCrc == canCrcRd && psMsgHead->srcDevType == HTE_MSG_DEV_PAD2)
                {
                    //校准过程中
                    if(PAD_CAN_CMD_TOUCH_PRESS == psMsgHead->cmd)
                    {
                        /*触摸屏按下*/
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
