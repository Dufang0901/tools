/*****************************************************************************
  FileName��    C_PadCalibrateForm.cpp
  Author:       zhulu
  Version:      1.0
  Date:         2015/12/8 13:28:57
  Description:  У�������
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

#define C_PAD_CALIBRATE_FILE         "/etc/pointercal"  //����У������Ϣ,�Ժ���ݸ���Ϣ���������
#define C_PAD_CALIBRATE_MAX_DIFF      10      //У��������ֵ


/*********************************************************
Function:       C_PadCalibrateForm::C_PadCalibrateForm
Description:    C_PadCalibrateForm ��Ĺ��캯��
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

    //����5��������

    //���Ͻ�
    mPointXY[0].setX(100);
    mPointXY[0].setY(100);

    //���Ͻ�
    mPointXY[1].setX(900);
    mPointXY[1].setY(100);

    //���½�
    mPointXY[2].setX(900);
    mPointXY[2].setY(650);

    //���½�
    mPointXY[3].setX(100);
    mPointXY[3].setY(650);

    //�м�-�ĵ����ڼ���У���Ƿ���Ч
    mPointXY[4].setX(500);
    mPointXY[4].setY(350);

    mPointFCnt = 0;

    //ֻ��ʾ���Ͻ�����,����������
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
Description:    C_PadCalibrateForm �����������
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
Description:    ���õ����AD�ĵ�ѹֵ
Param:          none
Return:         none
Other:          none
***********************************************************************/
void C_PadCalibrateForm::recvClickPointF(float x,float y)
{
    if(x == 0.0 || y == 0.0 || mPointFCnt > 4)
    {
        //��Ч����
        return ;
    }
    qDebug("touch screen pointxy x=%f,y=%f",x,y);

    mPointFXY[mPointFCnt].setX(x);
    mPointFXY[mPointFCnt].setY(y);

    if(0 == mPointFCnt)
    {
        //�������Ͻ�����,��ʾ���Ͻ�����
        ui->label00->setVisible(false);
        ui->label01->setVisible(false);
        ui->label10->setVisible(true);
        ui->label11->setVisible(true);
    }
    else if(1 == mPointFCnt)
    {
        //�������Ͻ�����,��ʾ���½�����
        ui->label10->setVisible(false);
        ui->label11->setVisible(false);

        ui->label20->setVisible(true);
        ui->label21->setVisible(true);
    }
    else if(2 == mPointFCnt)
    {
        //�������½�����,��ʾ���½�����
        ui->label20->setVisible(false);
        ui->label21->setVisible(false);

        ui->label30->setVisible(true);
        ui->label31->setVisible(true);
    }
    else if(3 == mPointFCnt)
    {
        //�������½�����,��ʾ�м�����
        ui->label30->setVisible(false);
        ui->label31->setVisible(false);

        ui->label40->setVisible(true);
        ui->label41->setVisible(true);
    }
    if(mPointFCnt >= 4)
    {
        //�����Ѿ�������,��ʼУ����Ļ
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
Description:    ����У��ֵ
Param:          none
Return:         �ɹ�����0,ʧ�ܷ���-1
Other:          none
***********************************************************************/
int C_PadCalibrateForm::calculation()
{
    //����  kFX0 + A = X0   kϵ��,A����,Ŀǰֻ����2����ȷ��,������ܴ�,�Ժ�ͨ��ƽ��ֵ��ȷ��,FX0��AD��ѹֵ,X0��Qt����ֵ
    //���Ͻ����������½�ȷ��X��
    mXiShuX = ((float)(mPointXY[0].x() - mPointXY[2].x()))/((float)(mPointFXY[0].x() - mPointFXY[2].x()));
    mConstX = mPointXY[0].x() - mXiShuX*mPointFXY[0].x();

    //���Ͻ����������½�ȷ��X��
    mXiShuY = ((float)(mPointXY[1].y() - mPointXY[3].y()))/((float)(mPointFXY[1].y() - mPointFXY[3].y()));
    mConstY = mPointXY[1].y() - mXiShuY*mPointFXY[1].y();


    //ͨ���м�����������У���ֵ�����������ķ�Χ��
    int tempX = mXiShuX*mPointFXY[4].x() + mConstX - mPointXY[4].x();
    int tempY = mXiShuY*mPointFXY[4].y() + mConstY - mPointXY[4].y();

    ui->label40->setVisible(false);
    ui->label41->setVisible(false);

    qDebug("touch screen calculation constX=%f,xishuX=%f,constY=%f,xishuY=%f",mConstX,mXiShuX,mConstY,mXiShuY);
    if(::abs(tempX) > C_PAD_CALIBRATE_MAX_DIFF || ::abs(tempY) > C_PAD_CALIBRATE_MAX_DIFF)
    {
        qDebug("touch screen calculation error x=%d,y=%d",::abs(tempX),::abs(tempY));

        //У��ʧ��,��ʾ������ʾ��,2s�����¿�ʼУ��
        ui->label_tips->setVisible(true);
        QTimer::singleShot(2*1000,this,SLOT(closeTips()));

        return -1;
    }
    return 0;
}


/***********************************************************************
Function:       C_PadCalibrateForm::saveCalibrateFile
Description:    ����У����Ϣ
Param:          none
Return:         �ɹ�����0,ʧ�ܷ���-1
Other:          none
***********************************************************************/
int C_PadCalibrateForm::saveCalibrateFile()
{
    //��ʾУ��Ľ���
    QFile file(C_PAD_CALIBRATE_FILE);
    if(file.open(QIODevice::WriteOnly))
    {
        //����У����Ϣ
        char tempBuf[128];
        ::sprintf(tempBuf,"%f,%f,%f,%f",mConstX,mXiShuX,mConstY,mXiShuY);
        file.write(tempBuf,strlen(tempBuf));
        file.close();

        return 0;
    }
    else
    {
        qDebug("touch screen calculation saveCalibrateFile error");

        //У��ʧ��,��ʾ������ʾ��,2s�����¿�ʼУ��
        ui->label_tips->setVisible(true);
        QTimer::singleShot(2*1000,this,SLOT(closeTips()));

        return -1;
    }
}


/***********************************************************************
Function:       C_PadCalibrateForm::closeTips
Description:    ������ʾ��,����У׼
Param:          none
Return:         none
Other:          none
***********************************************************************/
void C_PadCalibrateForm::closeTips()
{
    //����У��
    mPointFCnt = 0;

    //������ʾ��Ϣ
    ui->label_tips->setVisible(false);

    //��ʾ���Ͻ�����
    ui->label00->setVisible(true);
    ui->label01->setVisible(true);
}
