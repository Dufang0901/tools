#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "form.h"
#include "HteProtocol.h"

#include <QDebug>



typedef struct
{
    unsigned char name;
    unsigned char value;
}bicgpio_data;

static int xx = 0;
static int i = 0;
MainWindow* MainWindow::mMainWindow = NULL;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);//设置为无边框界面

    this->setCursor(Qt::ArrowCursor);
    ui->label_2->setText("点击次数:"+QString::number(i));

    f = new Form ;

    mTimer = new QTimer;
    connect(mTimer,SIGNAL(timeout()),this,SLOT(updateTime()));
    mTimer->start(1*1000);
}

MainWindow::~MainWindow()
{
    delete ui;
}

MainWindow* MainWindow::getInstance()
{
    if(mMainWindow == NULL)
    {
        mMainWindow = new MainWindow;
    }
    return mMainWindow;
}


void MainWindow::on_pushButton_clicked()
{
    i++;
    ui->label_2->setText("点击次数:"+QString::number(i));
    qDebug("点击次数:%d",i);

    qDebug("1111111111111111");
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    QPoint pos = event->pos();
    ui->label_1->setText("x="+QString::number(pos.x())+",y="+QString::number(pos.y()));    
    //qDebug()<<"move pos"<<pos;
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    QPoint pos = event->pos();
    ui->label_1->setText("x="+QString::number(pos.x())+",y="+QString::number(pos.y()));
    qDebug()<<"press pos"<<pos;
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    QPoint pos = event->pos();
    ui->label_1->setText("x="+QString::number(pos.x())+",y="+QString::number(pos.y()));
    qDebug()<<"release pos"<<pos;
}

void MainWindow::on_pushButton_2_clicked()
{
    qDebug("2222222222222222");
    f->show();
}


void MainWindow::updateTime()
{
    int fd;
    bicgpio_data opgpio;

    if(xx == 0)
    {
        xx = 1;
    }
    else
    {
        xx = 0;
    }

    fd = open("/dev/bicgpio", 0);
    if(fd < 0)
    {
       return;
    }
    opgpio.name = 1;
    opgpio.value = xx;
    ::ioctl(fd, 1, (void*)&opgpio);
    ::close(fd);
}
