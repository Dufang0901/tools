/*****************************************************************************
  FileName：    C_PadCalibrateForm.h
  Author:       zhulu
  Version:      1.0
  Date:         2015/12/8 13:28:57
  Description:  校验界面类
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

    QPoint  mPointXY[5];    //保存QT坐标值
    QPointF mPointFXY[5];   //保存AD XY电压值
    int     mPointFCnt;     //保存AD采样的个数(从0开始计算)

    float mConstX;  //X轴常量值
    float mConstY;  //Y轴常量值
    float mXiShuX;  //X轴系数比例值
    float mXiShuY;  //Y轴系数比例值
};

#endif // C_PADCALIBRATEFORM_H
