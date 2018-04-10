/*****************************************************************************
  FileName��    C_PadCalibrateForm.h
  Author:       zhulu
  Version:      1.0
  Date:         2015/12/8 13:28:57
  Description:  У�������
  History:
                <author>    <time>    <version>    <desc>
                 zhulu    2015.12.08    V1.0       create
*****************************************************************************/

#ifndef C_PADCALIBRATEFORM_H
#define C_PADCALIBRATEFORM_H

#include <QWidget>

#include "C_RecvCanThread.h"


namespace Ui {
class C_PadCalibrateForm;
}

class C_PadCalibrateForm : public QWidget
{
    Q_OBJECT
    
public:
    explicit C_PadCalibrateForm(QWidget *parent = 0);
    ~C_PadCalibrateForm();

    static C_PadCalibrateForm* getInstance();
private:
    int  calculation();
    int  saveCalibrateFile();

private slots:
    void recvClickPointF(float x,float y);
    void closeTips();
private:
    Ui::C_PadCalibrateForm *ui;
    C_RecvCanThread *mRecvCanThread;

    QPoint  mPointXY[5];    //����QT����ֵ
    QPointF mPointFXY[5];   //����AD XY��ѹֵ
    int     mPointFCnt;     //����AD�����ĸ���(��0��ʼ����)

    float mConstX;  //X�᳣��ֵ
    float mConstY;  //Y�᳣��ֵ
    float mXiShuX;  //X��ϵ������ֵ
    float mXiShuY;  //Y��ϵ������ֵ
};

#endif // C_PADCALIBRATEFORM_H
