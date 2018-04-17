#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMouseEvent>
#include <QTimer>

#include "form.h"



namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
    static MainWindow* getInstance();
private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();
    void updateTime();
private:
    Ui::MainWindow *ui;
    static MainWindow *mMainWindow;
    Form *f;
    QTimer *mTimer;
    // QWidget interface
protected:
    void mouseMoveEvent(QMouseEvent *event);

    // QWidget interface
protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
};

#endif // MAINWINDOW_H
