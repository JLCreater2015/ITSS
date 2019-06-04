#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtCore>
#include "welcome.h"
#include "etccarinfo.h"
#include "etccharge.h"
#include "etcrecharge.h"
#include "parksites.h"
#include "etcrundialog.h"
#include "parkcharge.h"
#include "parkuserinfo.h"
#include "parkrundialog.h"
#include "trafficflow.h"
#include "trafficlightmanage.h"
#include "trafficviolationrecord.h"
#include "trafficviolationdialog.h"
#include "MyUtils/loginusrsingleton.h"
#include "dialogs/logindialog.h"
#include "dialogs/registerdialog.h"
#include "aboutusdialog.h"
#include <QList>
#include <QWidget>
#include <QtNetwork>
#include <QStringList>
#include <QShowEvent>

//读取定时器计时间隔,
#define TIMER_INTERVAL 998

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
public:
    void loadImage();
    void init();
    void deletePointers();
    void createDBTables();
    void setColor();
    void initNetwork();

    void saveInfo(QString m);

    void saveParkInfo(QStringList tList,QString m, int v);
signals:
    void openLightWindowSign(int);
    void openListenWindowSignal1(const QString str);
    void openListenWindowSignal2(const QString str);
    void openListenWindowSignal3(const QString str);
private:
    void openSystem(bool b);
private slots:
    void newConnect();
    void readMessage();
    void displayError(QAbstractSocket::SocketError);

    void on_pushButton_carInfo_clicked();

    void on_pushButton_charge_clicked();

    void on_pushButton_recharge_clicked();

    void showEvent(QShowEvent *event);

    //获取当前时间
    void realTime();

    void clearWaring();

    void on_pushButton_psite_clicked();

    void on_pushButton_puser_clicked();

    void on_pushButton_pcharge_clicked();

    void on_pushButtonETC_clicked();

    void openButton(int v);

    void on_toolButton_exit_clicked();

    void on_pushButtonPark_clicked();

    void on_pushButton_record_clicked();

    void on_pushButton_light_clicked();

    void on_pushButton_building_clicked();

    void on_pushButton_traffic_flow_clicked();

    void on_pushButtonLight_clicked();

    void on_actionETCrun_triggered();

    void on_actionParkRun_triggered();

    void on_actionGOOLMonitoring_triggered();

    void on_actionOpen_triggered();

    void on_actionClose_triggered();

    void on_actionLogn_triggered();

    void on_actionExit_triggered();

    void on_actionRegist_triggered();

    void on_actionAbout_triggered();

    void on_toolButton_open_clicked();

    void on_toolButton_user_clicked();

    void loginSuccess(int v);

    void registerSuccess(int v);
private:
    Ui::MainWindow *ui;

private:
    ETCcharge *etcCharge;
    Welcome *welcome;
    EtcCarInfo *etcCarInfo;
    EtcRecharge *etcRecharge;
    ParkSites *parkSites;
    ETCRunDialog etcRunDialog;
    ParkCharge *parkCharge;
    ParkUserInfo *parkUserInfo;
    ParkRunDialog parkRunDialog;
    TrafficFlow *trafficFlow;
    TrafficLightManage *trafficLightManage,*buildingLightManage;
    TrafficViolationRecord *trafficViolationRecord;
    TrafficViolationDialog trafficViolationDialog;
    LogInUsrSingleton *logInUsrSingleton = LogInUsrSingleton::getInstance();
    LogInDialog logInDialog;
    RegisterDialog registerDialog;
    AboutUsDialog aboutUsDialog;

private:
    QTimer *realtimer;
    QTimer *waringtimer;
    QList<QWidget*> widgetList;

    QTcpSocket *m_tcpsocket;
    QTcpServer *m_tcpserver;

    int listenWindowSquence;

    bool isLonIn;//登录还是退出
    bool isOpen;//打开还是关闭
};

#endif // MAINWINDOW_H
