#include "form.h"
#include "ui_form.h"
#include "mainwindow.h"

Form::Form(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Form)
{
    ui->setupUi(this);
       this->setWindowFlags(Qt::FramelessWindowHint);//设置为无边框界面
}

Form::~Form()
{
    delete ui;
}

void Form::on_pushButton_clicked()
{
    this->hide();
    MainWindow::getInstance()->show();
    MainWindow::getInstance()->raise();
}


void Form::on_pushButton_1_clicked()
{
    ui->label->setText("1");
}


void Form::on_pushButton_2_clicked()
{
    ui->label->setText("2");
}


void Form::on_pushButton_3_clicked()
{
    ui->label->setText("3");
}


void Form::on_pushButton_4_clicked()
{
    ui->label->setText("4");
}


void Form::on_pushButton_5_clicked()
{
    ui->label->setText("5");
}


void Form::on_pushButton_6_clicked()
{
    ui->label->setText("6");
}


void Form::on_pushButton_7_clicked()
{
    ui->label->setText("7");
}


void Form::on_pushButton_8_clicked()
{
    ui->label->setText("8");
}

void Form::on_pushButton_9_clicked()
{
    ui->label->setText("9");
}

void Form::on_pushButton_calibrate_clicked()
{
    ::system("rm /etc/pointercal");
    ::system("sync");
    ::system("reboot");
}
