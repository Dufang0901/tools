/*****************************************************************************
  FileName��    C_RecvCanThread.h
  Author:       zhulu
  Version:      1.0
  Date:         2015/12/8 13:28:57
  Description:  ����can���ݵ���
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
}

class C_RecvCanThread : public QThread
{
    Q_OBJECT
public:
    explicit C_RecvCanThread(QObject *parent = 0);
    void run();

private:
    int  PadSetupUniputDevice();
    int  PadOpenCanDevice(void);
    void PadInitCalibrate();
    int  PadRecvCanMsg(int inCanFd,Uint8 *pOutMsg,Uint16 *pOutRdLen);
    void PadRecvCanMsgPro(Uint8 *pInMsg);
    void PadCanTouchClick(int inCode,int inValue);
    void PadCanTouchRelXY(int inPointX,int inPointY,int inPreX,int inPreY);
    void PadCanTouchAbsXY(int inPointX,int inPointY);
    void PadCanTouchSync();
private:
    int mCanFd;
    int mUinputFd;

    float mConstX;  //X�᳣��ֵ
    float mConstY;  //Y�᳣��ֵ
    float mXiShuX;  //X��ϵ������ֵ
    float mXiShuY;  //Y��ϵ������ֵ
};

#endif // _C_RECV_CAN_THREAD_H
