/*****************************************************************************
  FileName：    C_RecvCanThread.h
  Author:       zhulu
  Version:      1.0
  Date:         2015/12/8 13:28:57
  Description:  接收can数据的类
  History:
                <author>    <time>    <version>    <desc>
                 zhulu    2015.12.08    V1.0       create
*****************************************************************************/
#ifndef _C_RECV_CAN_THREAD_H
#define _C_RECV_CAN_THREAD_H

#include <QThread>

extern "C"
{
    #include "HteProtocol.h"
    #include "Rtc.h"
    #include "Rtc_RX8025.h"
}

class C_RecvCanThread : public QThread
{
    Q_OBJECT
public:
    explicit C_RecvCanThread(QObject *parent = 0);
    void run();

signals:
    void signalClickPointF(float x,float y);
private:
    int  PadSetupUniputDevice();
    int  PadOpenCanDevice(void);
    int  PadRecvCanMsg(int inCanFd,Uint8 *pOutMsg,Uint16 *pOutRdLen);
private:
    int mCanFd;
};

#endif // _C_RECV_CAN_THREAD_H

