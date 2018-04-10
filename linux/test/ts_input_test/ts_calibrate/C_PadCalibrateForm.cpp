/*****************************************************************************
  FileName：    C_PadCalibrateForm.cpp
  Author:       zhulu
  Version:      1.0
  Date:         2015/12/8 13:28:57
  Description:  校验界面类
  History:
                <author>    <time>    <version>    <desc>
                 zhulu    2015.12.08    V1.0       create
*****************************************************************************/
#include "C_PadCalibrateForm.h"
#include "ui_C_PadCalibrateForm.h"

#include <QTimer>
#include <QFile>
#include <QApplication>
#include <QDesktopWidget>

#define C_PAD_CALIBRATE_FILE         "/etc/pointercal"  //保存校验后的信息,以后根据该信息计算出坐标
#define C_PAD_CALIBRATE_MAX_DIFF      10      //校验最大误差值


/*********************************************************
Function:       C_PadCalibrateForm::C_PadCalibrateForm
Description:    C_PadCalibrateForm 类的构造函数
Param:          parent
Return:         none
Other:          none
*********************************************************/
C_PadCalibrateForm::C_PadCalibrateForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::C_PadCalibrateForm)
{
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    ui->setupUi(this);

    //界面5个点坐标

    //左上角
    mPointXY[0].setX(100);
    mPointXY[0].setY(100);

    //右上角
    mPointXY[1].setX(900);
    mPointXY[1].setY(100);

    //右下角
    mPointXY[2].setX(900);
    mPointXY[2].setY(650);

    //左下角
    mPointXY[3].setX(100);
    mPointXY[3].setY(650);

    //中间-改点用于计算校验是否有效
    mPointXY[4].setX(500);
    mPointXY[4].setY(350);

    mPointFCnt = 0;

    //只显示左上角坐标,其它的隐藏
    ui->label00->setVisible(true);
    ui->label01->setVisible(true);

    ui->label10->setVisible(false);
    ui->label11->setVisible(false);

    ui->label20->setVisible(false);
    ui->label21->setVisible(false);

    ui->label30->setVisible(false);
    ui->label31->setVisible(false);

    ui->label40->setVisible(false);
    ui->label41->setVisible(false);

    ui->label_tips->setVisible(false);

    qDebug("touch screen width=%d,height=%d",QApplication::desktop()->width(),QApplication::desktop()->height());
    for(int i = 0; i < 5; i++)
    {
        qDebug("touch screen point[%d] x=%d,y=%d",i,mPointXY[i].x(),mPointXY[i].y());
    }

    mRecvCanThread = new C_RecvCanThread;
    connect(mRecvCanThread,SIGNAL(signalClickPointF(float,float)),this,SLOT(recvClickPointF(float,float)));
    mRecvCanThread->start();

}

/*********************************************************
Function:       C_PadCalibrateForm::C_PadCalibrateForm
Description:    C_PadCalibrateForm 类的析构函数
Param:          parent
Return:         none
Other:          none
*********************************************************/
C_PadCalibrateForm::~C_PadCalibrateForm()
{
    delete ui;
}

/***********************************************************************
Function:       C_PadCalibrateForm::recvClickPointF
Description:    设置点击的AD的电压值
Param:          none
Return:         none
Other:          none
***********************************************************************/
void C_PadCalibrateForm::recvClickPointF(float x,float y)
{
    if(x == 0.0 || y == 0.0 || mPointFCnt > 4)
    {
        //无效数据
        return ;
    }
    qDebug("touch screen pointxy x=%f,y=%f",x,y);

    mPointFXY[mPointFCnt].setX(x);
    mPointFXY[mPointFCnt].setY(y);

    if(0 == mPointFCnt)
    {
        //隐藏左上角坐标,显示右上角坐标
        ui->label00->setVisible(false);
        ui->label01->setVisible(false);
        ui->label10->setVisible(true);
        ui->label11->setVisible(true);
    }
    else if(1 == mPointFCnt)
    {
        //隐藏右上角坐标,显示右下角坐标
        ui->label10->setVisible(false);
        ui->label11->setVisible(false);

        ui->label20->setVisible(true);
        ui->label21->setVisible(true);
    }
    else if(2 == mPointFCnt)
    {
        //隐藏右下角坐标,显示左下角坐标
        ui->label20->setVisible(false);
        ui->label21->setVisible(false);

        ui->label30->setVisible(true);
        ui->label31->setVisible(true);
    }
    else if(3 == mPointFCnt)
    {
        //隐藏左下角坐标,显示中间坐标
        ui->label30->setVisible(false);
        ui->label31->setVisible(false);

        ui->label40->setVisible(true);
        ui->label41->setVisible(true);
    }
    if(mPointFCnt >= 4)
    {
        //坐标已经点击完成,开始校验屏幕
        if(0 == calculation())
        {
            if(0 == saveCalibrateFile())
            {
                QApplication::exit();
            }
        }
    }
    mPointFCnt++;
}


/***********************************************************************
Function:       C_PadCalibrateForm::calculation
Description:    计算校验值
Param:          none
Return:         成功返回0,失败返回-1
Other:          none
***********************************************************************/
int C_PadCalibrateForm::calculation()
{
    //线性  kFX0 + A = X0   k系数,A常量,目前只采用2点来确定,如果误差很大,以后通过平均值来确定,FX0是AD电压值,X0是Qt坐标值
    //左上角坐标与右下角确定X轴
    mXiShuX = ((float)(mPointXY[0].x() - mPointXY[2].x()))/((float)(mPointFXY[0].x() - mPointFXY[2].x()));
    mConstX = mPointXY[0].x() - mXiShuX*mPointFXY[0].x();

    //右上角坐标与左下角确定X轴
    mXiShuY = ((float)(mPointXY[1].y() - mPointXY[3].y()))/((float)(mPointFXY[1].y() - mPointFXY[3].y()));
    mConstY = mPointXY[1].y() - mXiShuY*mPointFXY[1].y();


    //通过中间坐标来计算校验的值是在误差允许的范围内
    int tempX = mXiShuX*mPointFXY[4].x() + mConstX - mPointXY[4].x();
    int tempY = mXiShuY*mPointFXY[4].y() + mConstY - mPointXY[4].y();

    ui->label40->setVisible(false);
    ui->label41->setVisible(false);

    qDebug("touch screen calculation constX=%f,xishuX=%f,constY=%f,xishuY=%f",mConstX,mXiShuX,mConstY,mXiShuY);
    if(::abs(tempX) > C_PAD_CALIBRATE_MAX_DIFF || ::abs(tempY) > C_PAD_CALIBRATE_MAX_DIFF)
    {
        qDebug("touch screen calculation error x=%d,y=%d",::abs(tempX),::abs(tempY));

        //校验失败,显示错误提示框,2s后重新开始校验
        ui->label_tips->setVisible(true);
        QTimer::singleShot(2*1000,this,SLOT(closeTips()));

        return -1;
    }
    return 0;
}


/***********************************************************************
Function:       C_PadCalibrateForm::saveCalibrateFile
Description:    保存校验信息
Param:          none
Return:         成功返回0,失败返回-1
Other:          none
***********************************************************************/
int C_PadCalibrateForm::saveCalibrateFile()
{
    //显示校验的界面
    QFile file(C_PAD_CALIBRATE_FILE);
    if(file.open(QIODevice::WriteOnly))
    {
        //保存校验信息
        char tempBuf[128];
        ::sprintf(tempBuf,"%f,%f,%f,%f",mConstX,mXiShuX,mConstY,mXiShuY);
        file.write(tempBuf,strlen(tempBuf));
        file.close();

        return 0;
    }
    else
    {
        qDebug("touch screen calculation saveCalibrateFile error");

        //校验失败,显示错误提示框,2s后重新开始校验
        ui->label_tips->setVisible(true);
        QTimer::singleShot(2*1000,this,SLOT(closeTips()));

        return -1;
    }
}


/***********************************************************************
Function:       C_PadCalibrateForm::closeTips
Description:    隐藏提示框,重新校准
Param:          none
Return:         none
Other:          none
***********************************************************************/
void C_PadCalibrateForm::closeTips()
{
    //重新校验
    mPointFCnt = 0;

    //隐藏提示信息
    ui->label_tips->setVisible(false);

    //显示右上角坐标
    ui->label00->setVisible(true);
    ui->label01->setVisible(true);
}
