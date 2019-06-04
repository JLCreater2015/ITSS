#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtNetwork>
#include "posix_qextserialport.h"
//延时，TIME_OUT是串口读写的延时
#define TIME_OUT 10
//读取定时器计时间隔,500ms，读取定时器是我们读取串口缓存的延时
#define TIMER_INTERVAL 200
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();
    void readMyCom();
    void closeDJ();
public:
    void openDJ(int angle);
protected:
    void setButtonEnabled(bool status);
public:
    void init();
private:
    Ui::MainWindow *ui;
    QTcpSocket *m_tcpsocket;
    Posix_QextSerialPort  *myCom;
    QTimer *timer;
    unsigned int timerdly;

    QTimer *writtimer;
    bool isCome;

};

#endif // MAINWINDOW_H
