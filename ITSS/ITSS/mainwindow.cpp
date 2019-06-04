#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "variabledef.h"
#include <QDebug>
#include <QMovie>
#include <QtSql>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QMessageBox>
#include <QInputDialog>
#include "MyUtils/loginusrsingleton.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //默认关闭系统
    openSystem(false);
    ui->actionClose->setEnabled(false);
    ui->actionExit->setEnabled(false);
    ui->toolButton_open->setToolTip("打开系统");
    //初始化
    init();
    initNetwork();
    QObject::connect(&etcRunDialog,SIGNAL(closeSign1(int)),this,SLOT(openButton(int)));
    QObject::connect(&parkRunDialog,SIGNAL(closeSign2(int)),this,SLOT(openButton(int)));
    QObject::connect(&logInDialog,SIGNAL(closeSign4(int)),this,SLOT(loginSuccess(int)));
    QObject::connect(&registerDialog,SIGNAL(closeSign5(int)),this,SLOT(registerSuccess(int)));
    QObject::connect(&trafficViolationDialog,SIGNAL(closeSign3(int)),this,SLOT(openButton(int)));
    QObject::connect(this,SIGNAL(openLightWindowSign(int)),trafficLightManage,SLOT(setLightType(int)));
    QObject::connect(this,SIGNAL(openLightWindowSign(int)),buildingLightManage,SLOT(setLightType(int)));
    //createDBTables();

    listenWindowSquence = 0;

    QObject::connect(this,SIGNAL(openListenWindowSignal1(const QString)),&etcRunDialog,SLOT(showInfoIntime(QString)));
    QObject::connect(this,SIGNAL(openListenWindowSignal2(const QString)),&parkRunDialog,SLOT(showInfoIntime(QString)));
    QObject::connect(this,SIGNAL(openListenWindowSignal3(const QString)),&trafficViolationDialog,SLOT(showInfoIntime(QString)));
}

void MainWindow::createDBTables()
{
    QSqlDatabase db;
    if(QSqlDatabase::contains("myConnection"))
        db = QSqlDatabase::database("myConnection");
    else
        db = QSqlDatabase::addDatabase("QSQLITE","myConnection");//添加数据库驱动
    db.setDatabaseName(DBPath);
    if(!db.open()) //打开数据库
    {
        QMessageBox::critical(0, qApp->tr("Cannot open database"),
                              qApp->tr("Unable to establish a database connection. "),
                              QMessageBox::Cancel);
        return;
    }
    QSqlQuery query(db);
    query.exec(tr("CREATE TABLE login_record(id INTEGER PRIMARY KEY AUTOINCREMENT,name VARCHAR(30) "
                  "NOT NULL,user_type INT NOT NULL,login_time datetime NOT NULL)"));
    query.finish();
    db.close();
}

void MainWindow::init()
{
    this->welcome = new Welcome(this);
    this->etcCharge = new ETCcharge(this);
    this->etcRecharge = new EtcRecharge(this);
    this->etcCarInfo = new EtcCarInfo(this);
    this->parkSites = new ParkSites(this);
    this->parkCharge = new ParkCharge(this);
    this->parkUserInfo = new ParkUserInfo(this);
    this->trafficFlow = new TrafficFlow(this);
    this->trafficLightManage = new TrafficLightManage(this);
    this->trafficViolationRecord = new TrafficViolationRecord(this);
    this->buildingLightManage = new TrafficLightManage(this);
    deletePointers();
    loadImage();
    realtimer = new QTimer(this);
    waringtimer = new QTimer(this);
    connect(realtimer, SIGNAL(timeout()), this, SLOT(realTime()));
    connect(waringtimer, SIGNAL(timeout()), this, SLOT(clearWaring()));
    //realtimer->start(TIMER_INTERVAL);

    isLonIn = false;
    isOpen = false;
}

void MainWindow::showEvent(QShowEvent *event)
{
   // openSystem(false);
   // ui->actionClose->setEnabled(false);
   // ui->actionExit->setEnabled(false);
   // on_toolButton_open_clicked();
}

void MainWindow::initNetwork()
{
    m_tcpserver = new QTcpServer(this);
    m_tcpsocket = new QTcpSocket(this);
    m_tcpserver->listen(QHostAddress::AnyIPv4,6666);//监听的端口号
    //AnyIPv4
    connect(m_tcpserver,SIGNAL(newConnection()),this,SLOT(newConnect()));
    connect(m_tcpsocket,SIGNAL(error(QAbstractSocket::SocketError)),
                  this,SLOT(displayError(QAbstractSocket::SocketError)));
}

void MainWindow::newConnect()
{
    m_tcpsocket = m_tcpserver->nextPendingConnection();//设置监听
    connect(m_tcpsocket,SIGNAL(readyRead()),
            this,SLOT(readMessage())); //服务器接收客户端的消息
    connect(m_tcpsocket,SIGNAL(disconnected()),
                m_tcpsocket,SLOT(deleteLater()));
}

void MainWindow::displayError(QAbstractSocket::SocketError)
{
   qDebug()<<m_tcpsocket->errorString();
}

//接受网络数据
void MainWindow::readMessage()
{
    QString message=m_tcpsocket->readAll();
    //坑：
    /*QDataStream in(m_tcpsocket);

    in.setVersion(QDataStream::Qt_5_7);

    in >> message;*/
    if(!message.isEmpty())
    {
        saveInfo(message);
        qDebug()<<message;
        message = "";     
    }
    m_tcpsocket->close();/////////释放Socket资源
}

//保存数据
void MainWindow::saveInfo(QString m)
{
    QSqlDatabase db;
    if(QSqlDatabase::contains("myConnection_main"))
        db = QSqlDatabase::database("myConnection_main");
    else
        db = QSqlDatabase::addDatabase("QSQLITE","myConnection_main");//添加数据库驱动
    db.setDatabaseName(DBPath);
    if(!db.open()) //打开数据库
    {
        QMessageBox::critical(0, qApp->tr("Cannot open database"),
                              qApp->tr("Unable to establish a database connection. "),
                              QMessageBox::Cancel);
        return;
    }
    QSqlQuery query(db);
    QDateTime current_date_time = QDateTime::currentDateTime();
    QString cdt = current_date_time.toString("yyyy-MM-dd hh:mm:ss");
    if("1" == m.split("#").at(0)){
        query.prepare("SELECT name,carNum,carType from users where tagID=?");
        query.addBindValue(m.split("#").at(1));
        if(!query.exec())
            return;

        QStringList list1;
        while(query.next())
        {
            int fieldNo1 = query.record().indexOf("carNum");
            int fieldNo2 = query.record().indexOf("carType");
            int fieldNo3 = query.record().indexOf("name");
            QString v1 = query.value(fieldNo1).toString();
            QString v2 = query.value(fieldNo2).toString();
            QString v3 = query.value(fieldNo3).toString();
            list1.append(v3);
            list1.append(v1);
            list1.append(v2);
        }
        if(list1.isEmpty() || list1.length() < 3)
            return;

        query.prepare("SELECT price FROM cartypes WHERE cartype=?");
        query.addBindValue(list1.at(2));
        if(!query.exec())
            return;
        while(query.next())
        {
            int fieldNo4 = query.record().indexOf("price");
            QString v4 = query.value(fieldNo4).toString();
            list1.append(v4);
        }
        query.finish();
        if(list1.isEmpty() || list1.length() < 4)
            return;

        query.prepare("INSERT INTO records(name,carNum,tagID,amount,recordType,date) VALUES(?,?,?,?,?,?)");
        query.addBindValue(list1.first());
        query.addBindValue(list1.at(1));
        query.addBindValue(m.split("#").at(1));
        query.addBindValue(list1.at(3));
        query.addBindValue(tr("ETC消费"));
        query.addBindValue(cdt);
        if(query.exec()){         
            if(1 == listenWindowSquence)
                emit openListenWindowSignal1(m);
            //listenWindowSquence = 0;
        }
    }else if("2" == m.split("#").first()){
        QStringList myList;
        myList.append(cdt);//1.date
        myList.append(m.split("#").at(2));//2.cardID
        query.prepare("SELECT userName,carNumber,carType FROM user_parking WHERE cardID=?");
        query.addBindValue(m.split("#").at(2));
        if(!query.exec())
            return;
        QStringList list1;
        while(query.next())
        {
            int fieldNo1 = query.record().indexOf("userName");
            int fieldNo2 = query.record().indexOf("carNumber");
            int fieldNo3 = query.record().indexOf("carType");
            QString v1 = query.value(fieldNo1).toString();
            QString v2 = query.value(fieldNo2).toString();
            QString v3 = query.value(fieldNo3).toString();
            list1.append(v1);//1
            list1.append(v2);//2
            list1.append(v3);//3
            myList.append(v1);//3.userName
            myList.append(v2);//4.carNumber
            myList.append(v3);//5.carType
        }
        if(list1.isEmpty() || list1.length() < 3)
            return;
        if(m.split("#").at(1) == "comeIn")
        {
            query.prepare("SELECT price FROM cartypes WHERE cartype=?");
            query.addBindValue(list1.at(2));
            if(!query.exec())
                return;
            while(query.next())
            {
                int fieldNo4 = query.record().indexOf("price");
                QString v4 = query.value(fieldNo4).toString();
                myList.append(v4);//6.unitMoney
            }
            //query.finish();

            QString site;
            query.prepare("SELECT leftSite FROM sites");
            if(!query.exec())
                return;
            while(query.next())
            {
                int fieldNo8 = query.record().indexOf("leftSite");
                site = query.value(fieldNo8).toString();
            }
            query.prepare("UPDATE sites SET leftSite=? WHERE leftSite=?");
            bool ok;
            query.addBindValue(QString::number(site.toInt(&ok,10)-1,10));
            query.addBindValue(site);
            if(!query.exec())
                return;
            saveParkInfo(myList,m,0);
        }
        else
        {
            query.prepare("SELECT startTime,unitPrice FROM enter_car WHERE carNumber=?");
            query.addBindValue(list1.at(1));
            if(!query.exec())
                return;
            while(query.next())
            {
                int fieldNo5 = query.record().indexOf("startTime");
                QString v5 = query.value(fieldNo5).toString();
                int fieldNo6 = query.record().indexOf("unitPrice");
                QString v6 = query.value(fieldNo6).toString();
                myList.append(v5);//6.startTime
                myList.append(v6);//7.unitMoney
            }
            //query.finish();
            if(myList.isEmpty() || myList.length() < 7)
                return;
            QDateTime qdt1,qdt2;
            qdt1 = QDateTime::fromString(myList.at(5), "yyyy-MM-dd hh:mm:ss");
            qdt2 = QDateTime::fromString(myList.at(0), "yyyy-MM-dd hh:mm:ss");
            QTime time1 = qdt1.time(),time2 = qdt2.time();
            QDate date1 = qdt1.date(),date2 = qdt2.date();
            int a = time2.hour()-time1.hour();
            int b = date2.day()-date1.day();
            int c = b * 24 + a + 1;
            bool ok;
            int d = myList.at(6).toInt(&ok,10) * c;
            myList.append(QString::number(d,10));//8.money
            QString site;
            query.prepare("SELECT leftSite FROM sites");
            if(!query.exec())
                return;
            while(query.next())
            {
                int fieldNo9 = query.record().indexOf("leftSite");
                site = query.value(fieldNo9).toString();
            }
            query.prepare("UPDATE sites SET leftSite=?");
            query.addBindValue(QString::number(site.toInt(&ok,10)+1,10));
            if(!query.exec())
                return;
            saveParkInfo(myList,m,1);
        }
    }else if("3" == m.split("#").first()){
        query.prepare("SELECT name,carNum FROM users WHERE tagID=?");
        query.addBindValue(m.split("#").at(1));
        if(!query.exec())
            return;

        QStringList list1;
        while(query.next())
        {
            int fieldNo1 = query.record().indexOf("carNum");
            int fieldNo2 = query.record().indexOf("name");
            QString v1 = query.value(fieldNo1).toString();
            QString v2 = query.value(fieldNo2).toString();
            list1.append(v1);
            list1.append(v2);
        }
        if(list1.isEmpty() || list1.length() < 2)
            return;

        ////////
        ui->labelWarning->setText(list1.first()+" 违章!");
        waringtimer->start(10000);
        ////////

        query.prepare("INSERT INTO car_distribute(owner_name,carNum,photographTime) VALUES(?,?,?)");
        query.addBindValue(list1.at(1));
        query.addBindValue(list1.at(0));
        query.addBindValue(cdt);
        if(query.exec()){
            if(3 == listenWindowSquence)
                emit openListenWindowSignal3(m);
            //listenWindowSquence = 0;
        }
    }
    query.finish();
    if(db.open())
        db.close();
}

void MainWindow::clearWaring()
{
    ui->labelWarning->setText("");
    waringtimer->stop();
}

void MainWindow::saveParkInfo(QStringList tList, QString m,int v)
{
    QSqlDatabase db;
    if(QSqlDatabase::contains("myConnection_main0"))
        db = QSqlDatabase::database("myConnection_main0");
    else
        db = QSqlDatabase::addDatabase("QSQLITE","myConnection_main0");//添加数据库驱动
    db.setDatabaseName(DBPath);
    if(!db.open()) //打开数据库
    {
        QMessageBox::critical(0, qApp->tr("Cannot open database"),
                              qApp->tr("Unable to establish a database connection. "),
                              QMessageBox::Cancel);
        return;
    }
    QSqlQuery query(db);
    if(v == 0)
    {
        if(tList.isEmpty() || tList.length() < 6)
            return;
        query.prepare("INSERT INTO enter_car(carNumber,startTime,unitPrice) VALUES(?,?,?)");
        query.addBindValue(tList.at(3));
        query.addBindValue(tList.at(0));
        query.addBindValue(tList.at(5));
        if(query.exec()){
            if(2 == listenWindowSquence)
                emit openListenWindowSignal2(m);
            //listenWindowSquence = 0;
        }
    }
    else
    {
        if(tList.isEmpty() || tList.length() < 8)
            return;
        query.prepare("INSERT INTO pay_records(carNumber,startTime,endTime,payMoney,unitPrice) VALUES(?,?,?,?,?)");
        query.addBindValue(tList.at(3));
        query.addBindValue(tList.at(5));
        query.addBindValue(tList.at(0));
        query.addBindValue(tList.at(7));
        query.addBindValue(tList.at(6));
        if(query.exec()){
            if(2 == listenWindowSquence)
                emit openListenWindowSignal2(m);
            else
            {
                query.prepare("DELETE FROM enter_car WHERE carNumber=?");
                query.addBindValue(tList.at(3));
                query.exec();
            }
            //listenWindowSquence = 0;
        }
    }
    query.finish();
    if(db.open())
        db.close();
}

void MainWindow::realTime()
{
    QDateTime current_date_time = QDateTime::currentDateTime();
    QString cdt = current_date_time.toString("MM-dd hh:mm:ss ddd");
    ui->textEdit_time->setText(cdt);
}

void MainWindow::loadImage()
{
    //显示图片
    QMovie *movie = new QMovie(MoviePath);
    ui->label_welcome->setMovie(movie);
    movie->start();
    ui->label->setText("欢迎使用该系统");
    this->welcome->move(249,114);
    this->welcome->show();
    widgetList.append(welcome);
}

void MainWindow::setColor()
{
    ui->pushButton_recharge->setStyleSheet("background-color: rgb(199, 188, 255);");
    ui->pushButton_record->setStyleSheet("background-color: rgb(199, 188, 255);");
    ui->pushButton_traffic_flow->setStyleSheet("background-color: rgb(199, 188, 255);");
    ui->pushButton_building->setStyleSheet("background-color: rgb(199, 188, 255);");
    ui->pushButton_carInfo->setStyleSheet("background-color: rgb(199, 188, 255);");
    ui->pushButton_charge->setStyleSheet("background-color: rgb(199, 188, 255);");
    ui->pushButton_light->setStyleSheet("background-color: rgb(199, 188, 255);");
    ui->pushButton_pcharge->setStyleSheet("background-color: rgb(199, 188, 255);");
    ui->pushButton_psite->setStyleSheet("background-color: rgb(199, 188, 255);");
    ui->pushButton_puser->setStyleSheet("background-color: rgb(199, 188, 255);");
}

void MainWindow::deletePointers()
{
    if(!this->welcome->isHidden())
        this->welcome->hide();
    if(!this->etcCarInfo->isHidden())
        this->etcCarInfo->hide();
    if(!this->etcCharge->isHidden())
        this->etcCharge->hide();
    if(!this->etcRecharge->isHidden())
        this->etcRecharge->hide();
    if(!this->parkSites->isHidden())
        this->parkSites->hide();
    if(!this->parkCharge->isHidden())
        this->parkCharge->hide();
    if(!this->parkUserInfo->isHidden())
        this->parkUserInfo->hide();
    if(!this->trafficFlow->isHidden())
        this->trafficFlow->hide();
    if(!this->trafficLightManage->isHidden())
        this->trafficLightManage->hide();
    if(!this->trafficViolationRecord->isHidden())
        this->trafficViolationRecord->hide();
    if(!this->buildingLightManage->isHidden())
        this->buildingLightManage->hide();
}

void MainWindow::openButton(int v)
{
    if(v == 0)
    {
        ui->pushButtonETC->setEnabled(true);
        ui->actionETCrun->setEnabled(true);
    }
    else if(v == 1)
    {
        ui->pushButtonPark->setEnabled(true);
        ui->actionParkRun->setEnabled(true);
    }
    else if(v == 2)
    {
        ui->pushButtonLight->setEnabled(true);
        ui->actionGOOLMonitoring->setEnabled(true);
    }
    else
    {
        ui->pushButtonETC->setEnabled(true);
        ui->pushButtonPark->setEnabled(true);
        ui->pushButtonLight->setEnabled(true);
        ui->actionGOOLMonitoring->setEnabled(true);
        ui->actionParkRun->setEnabled(true);
        ui->actionETCrun->setEnabled(true);
    }
    listenWindowSquence = 0;
}
//打开或关闭系统
void MainWindow::openSystem(bool b)
{
    ui->actionETCrun->setEnabled(b);
    ui->actionGOOLMonitoring->setEnabled(b);
    ui->actionParkRun->setEnabled(b);
    ui->toolButton_exit->setEnabled(b);
    ui->pushButtonETC->setEnabled(b);
    ui->pushButtonLight->setEnabled(b);
    ui->pushButtonPark->setEnabled(b);
    ui->pushButton_building->setEnabled(b);
    ui->pushButton_carInfo->setEnabled(b);
    ui->pushButton_charge->setEnabled(b);
    ui->pushButton_light->setEnabled(b);
    ui->pushButton_pcharge->setEnabled(b);
    ui->pushButton_psite->setEnabled(b);
    ui->pushButton_puser->setEnabled(b);
    ui->pushButton_recharge->setEnabled(b);
    ui->pushButton_record->setEnabled(b);
    ui->pushButton_traffic_flow->setEnabled(b);
}

MainWindow::~MainWindow()
{
    widgetList.clear();
    if(this->welcome)
        delete this->welcome;
    if(this->etcCarInfo)
        delete this->etcCarInfo;
    if(this->etcCharge)
        delete this->etcCharge;
    if(this->etcRecharge)
        delete this->etcRecharge;
    if(this->parkSites)
        delete this->parkSites;
    if(this->parkCharge)
        delete this->parkCharge;
    if(this->parkUserInfo)
        delete this->parkUserInfo;
    if(this->trafficFlow)
        delete this->trafficFlow;
    if(!this->trafficLightManage)
        delete this->trafficLightManage;
    if(!this->trafficViolationRecord)
        delete this->trafficViolationRecord;
    if(!this->buildingLightManage)
        delete this->buildingLightManage;

    delete ui;
}

void MainWindow::on_pushButton_carInfo_clicked()
{
    deletePointers();
    setColor();
    ui->pushButton_carInfo->setStyleSheet("background-color: rgb(232, 232, 232);");
    ui->label->setText("ETC车辆收费管理");
    this->etcCarInfo->move(249,114);
    this->etcCarInfo->show();
    widgetList.append(etcCarInfo);
}

void MainWindow::on_pushButton_charge_clicked()
{
    deletePointers();
    setColor();
    ui->pushButton_charge->setStyleSheet("background-color: rgb(232, 232, 232);");
    ui->label->setText("ETC收费记录");
    this->etcCharge->move(249,114);
    this->etcCharge->show();
    widgetList.append(etcCharge);
}

void MainWindow::on_pushButton_recharge_clicked()
{
    deletePointers();
    setColor();
    ui->pushButton_recharge->setStyleSheet("background-color: rgb(232, 232, 232);");
    ui->label->setText("用户充值中心");
    this->etcRecharge->move(249,114);
    this->etcRecharge->show();
    widgetList.append(etcRecharge);
}

void MainWindow::on_pushButton_psite_clicked()
{
    deletePointers();
    setColor();
    ui->pushButton_psite->setStyleSheet("background-color: rgb(232, 232, 232);");
    ui->label->setText("停车场车位管理");
    this->parkSites->move(249,114);
    this->parkSites->show();
    widgetList.append(parkSites);
}

void MainWindow::on_pushButton_puser_clicked()
{
    deletePointers();
    setColor();
    ui->pushButton_puser->setStyleSheet("background-color: rgb(232, 232, 232);");
    ui->label->setText("停车场用户管理");
    this->parkUserInfo->move(249,114);
    this->parkUserInfo->show();
    widgetList.append(parkUserInfo);
}

void MainWindow::on_pushButton_pcharge_clicked()
{
    deletePointers();
    setColor();
    ui->pushButton_pcharge->setStyleSheet("background-color: rgb(232, 232, 232);");
    ui->label->setText("停车场收费管理");
    this->parkCharge->move(249,114);
    this->parkCharge->show();
    widgetList.append(parkCharge);
}

void MainWindow::on_pushButton_record_clicked()
{
    deletePointers();
    setColor();
    ui->pushButton_record->setStyleSheet("background-color: rgb(232, 232, 232);");
    ui->label->setText("交通违章记录");
    this->trafficViolationRecord->move(249,114);
    this->trafficViolationRecord->show();
    widgetList.append(trafficViolationRecord);
}

void MainWindow::on_pushButton_light_clicked()
{
    deletePointers();
    setColor();
    emit openLightWindowSign(0);//不能放在后面
    ui->pushButton_light->setStyleSheet("background-color: rgb(232, 232, 232);");
    ui->label->setText("交通灯管理");
    this->trafficLightManage->move(249,114);
    this->trafficLightManage->show();
    widgetList.append(trafficLightManage);
}

void MainWindow::on_pushButton_building_clicked()
{
    deletePointers();
    setColor();
    emit openLightWindowSign(1);
    ui->pushButton_building->setStyleSheet("background-color: rgb(232, 232, 232);");
    ui->label->setText("建筑灯管理");
    this->buildingLightManage->move(249,114);
    this->buildingLightManage->show();
    widgetList.append(buildingLightManage);
}

void MainWindow::on_pushButton_traffic_flow_clicked()
{
    deletePointers();
    setColor();
    ui->pushButton_traffic_flow->setStyleSheet("background-color: rgb(232, 232, 232);");
    ui->label->setText("车流量检测");
    this->trafficFlow->move(249,114);
    this->trafficFlow->show();
    widgetList.append(trafficFlow);
}

void MainWindow::on_pushButtonETC_clicked()
{
    //etcRunDialog = new ETCRunDialog(this) ;
    etcRunDialog.show();
    listenWindowSquence = 1;
    ui->pushButtonETC->setEnabled(false);
    ui->actionETCrun->setEnabled(false);
}

void MainWindow::on_pushButtonPark_clicked()
{
    parkRunDialog.show();
    listenWindowSquence = 2;
    ui->pushButtonPark->setEnabled(false);
    ui->actionParkRun->setEnabled(false);
}

void MainWindow::on_pushButtonLight_clicked()
{
    trafficViolationDialog.show();
    listenWindowSquence = 3;
    ui->pushButtonLight->setEnabled(false);
    ui->actionGOOLMonitoring->setEnabled(false);
}

void MainWindow::on_toolButton_exit_clicked()
{
    deletePointers();
    setColor();
    if(!widgetList.isEmpty())
    {
        QWidget *qw = widgetList.last();
        if(qw == etcCarInfo)
        {
            ui->pushButton_carInfo->setStyleSheet("background-color: rgb(232, 232, 232);");
            ui->label->setText("ETC车辆收费管理");
        }
        else if(qw == etcCharge)
        {
            ui->pushButton_charge->setStyleSheet("background-color: rgb(232, 232, 232);");
            ui->label->setText("ETC收费记录");
        }
        else if(qw == etcRecharge)
        {
            ui->pushButton_recharge->setStyleSheet("background-color: rgb(232, 232, 232);");
            ui->label->setText("用户充值中心");
        }
        else if(qw == parkSites)
        {
            ui->pushButton_psite->setStyleSheet("background-color: rgb(232, 232, 232);");
            ui->label->setText("停车场车位管理");
        }
        else if(qw == parkCharge)
        {
            ui->pushButton_pcharge->setStyleSheet("background-color: rgb(232, 232, 232);");
            ui->label->setText("停车场收费管理");
        }
        else if(qw == parkUserInfo)
        {
            ui->pushButton_puser->setStyleSheet("background-color: rgb(232, 232, 232);");
            ui->label->setText("停车场用户管理");
        }
        else if(qw == trafficFlow)
        {
            ui->pushButton_traffic_flow->setStyleSheet("background-color: rgb(232, 232, 232);");
            ui->label->setText("车流量检测");
        }
        else if(qw == buildingLightManage)
        {
            ui->pushButton_building->setStyleSheet("background-color: rgb(232, 232, 232);");
            ui->label->setText("建筑灯管理");
            emit openLightWindowSign(1);
        }
        else if(qw == trafficViolationRecord)
        {
            ui->pushButton_record->setStyleSheet("background-color: rgb(232, 232, 232);");
            ui->label->setText("交通违章记录");
        }
        else if(qw == trafficLightManage)
        {
            ui->pushButton_light->setStyleSheet("background-color: rgb(232, 232, 232);");
            ui->label->setText("交通灯管理");
            emit openLightWindowSign(0);
        }
        else
            ui->label->setText("欢迎使用该系统");
        widgetList.removeLast();
        qw->move(249,114);
        qw->show();
    }else
    {
        ui->label->setText("欢迎使用该系统");
        this->welcome->move(249,114);
        this->welcome->show();
        widgetList.append(welcome);
    }
}
//菜单按钮
void MainWindow::on_actionETCrun_triggered()
{
    etcRunDialog.show();
    listenWindowSquence = 1;
    ui->pushButtonETC->setEnabled(false);
    ui->actionETCrun->setEnabled(false);
}

void MainWindow::on_actionParkRun_triggered()
{
    parkRunDialog.show();
    listenWindowSquence = 2;
    ui->pushButtonPark->setEnabled(false);
    ui->actionParkRun->setEnabled(false);
}

void MainWindow::on_actionGOOLMonitoring_triggered()
{
    trafficViolationDialog.show();
    listenWindowSquence = 3;
    ui->pushButtonLight->setEnabled(false);
    ui->actionGOOLMonitoring->setEnabled(false);
}

void MainWindow::on_actionOpen_triggered()
{
    logInDialog.show();
}

void MainWindow::on_actionClose_triggered()
{
    QMessageBox *msgBox;
    msgBox = new QMessageBox(tr("Confirm"),       ///--这里是设置消息框标题
        "确认要退出并关闭系统吗?",                       ///--这里是设置消息框显示的内容
        QMessageBox::Warning,                          ///--这里是在消息框显示的图标
        QMessageBox::Yes | QMessageBox::Default,     ///---这里是显示消息框上的按钮情况
        QMessageBox::Cancel | QMessageBox::Escape,  ///---这里与 键盘上的 escape 键结合。当用户按下该键，消息框将执行cancel按钮事件
        0);                                                     ///---这里是 定义第三个按钮， 该例子 只是 了显示2个按钮

    msgBox->show();
    if(msgBox->exec() == QMessageBox::Yes)
    {     
        LogInUsrSingleton *pSingleton1 = LogInUsrSingleton::getInstance();
        pSingleton1->setuserName("admin");
        pSingleton1->setauthority(0);
        realtimer->stop();//关闭定时器
        ui->textEdit_time->setText("");
        isLonIn = false;
        isOpen = false;
        this->close();
    }
}

void MainWindow::on_actionLogn_triggered()
{
    logInDialog.show();
}

void MainWindow::loginSuccess(int v)
{
    if(v == 1)
    {
        openSystem(true);
        ui->toolButton_user->setToolTip("退出登录");
        ui->toolButton_open->setToolTip("关闭系统");
        ui->actionOpen->setEnabled(false);
        ui->actionLogn->setEnabled(false);
        ui->actionExit->setEnabled(true);
        ui->actionClose->setEnabled(true);
        realtimer->start(TIMER_INTERVAL);
        isLonIn = true;
        isOpen = true;
    }else if(v == 2)
    {
        isLonIn = false;
        isOpen = false;
        registerDialog.show();
    }else{
        openSystem(false);
        ui->toolButton_user->setToolTip("登录");
        ui->toolButton_open->setToolTip("打开系统");
        ui->actionOpen->setEnabled(true);
        ui->actionLogn->setEnabled(true);
        ui->actionExit->setEnabled(false);
        ui->actionClose->setEnabled(false);
        isLonIn = false;
        isOpen = false;
    }
}

void MainWindow::registerSuccess(int v)
{
    if(v == 1)
    {
        LogInUsrSingleton *pSingleton1 = LogInUsrSingleton::getInstance();
        if(pSingleton1->getuserName() == "admin" && pSingleton1->getauthority() == 0)//没有用户登录
        {
            logInDialog.show();
        }else{
            QMessageBox *msgBox;
            msgBox = new QMessageBox(tr("Confirm"),       ///--这里是设置消息框标题
                "要退出并重新登录吗?",                       ///--这里是设置消息框显示的内容
                QMessageBox::Information,                          ///--这里是在消息框显示的图标
                QMessageBox::Yes | QMessageBox::Default,     ///---这里是显示消息框上的按钮情况
                QMessageBox::Cancel | QMessageBox::Escape,  ///---这里与 键盘上的 escape 键结合。当用户按下该键，消息框将执行cancel按钮事件
                0);                                                     ///---这里是 定义第三个按钮， 该例子 只是 了显示2个按钮

            msgBox->show();
            if(msgBox->exec() == QMessageBox::Yes)
            {
                LogInUsrSingleton *pSingleton1 = LogInUsrSingleton::getInstance();
                pSingleton1->setuserName("admin");
                pSingleton1->setauthority(0);
                openSystem(false);
                ui->actionClose->setEnabled(false);
                ui->actionExit->setEnabled(false);
                ui->actionLogn->setEnabled(true);
                ui->actionOpen->setEnabled(true);
                realtimer->stop();//关闭定时器
                ui->textEdit_time->setText("");
                logInDialog.show();
            }
        }
    }
}

void MainWindow::on_actionExit_triggered()
{   
    QMessageBox *msgBox;
    msgBox = new QMessageBox(tr("Confirm"),       ///--这里是设置消息框标题
        "确认要退出吗?",                       ///--这里是设置消息框显示的内容
        QMessageBox::Warning,                          ///--这里是在消息框显示的图标
        QMessageBox::Yes | QMessageBox::Default,     ///---这里是显示消息框上的按钮情况
        QMessageBox::Cancel | QMessageBox::Escape,  ///---这里与 键盘上的 escape 键结合。当用户按下该键，消息框将执行cancel按钮事件
        0);                                                     ///---这里是 定义第三个按钮， 该例子 只是 了显示2个按钮

    msgBox->show();
    if(msgBox->exec() == QMessageBox::Yes)
    {
        LogInUsrSingleton *pSingleton1 = LogInUsrSingleton::getInstance();
        pSingleton1->setuserName("admin");
        pSingleton1->setauthority(0);
        openSystem(false);
        ui->actionClose->setEnabled(false);
        ui->actionExit->setEnabled(false);
        ui->actionLogn->setEnabled(true);
        ui->actionOpen->setEnabled(true);
        realtimer->stop();//关闭定时器
        ui->textEdit_time->setText("");
        isLonIn = false;
        isOpen = false;
    }
}

void MainWindow::on_actionRegist_triggered()
{
    registerDialog.show();
}

void MainWindow::on_actionAbout_triggered()
{
    aboutUsDialog.show();
}

void MainWindow::on_toolButton_open_clicked()
{
    if(isOpen){
        on_actionClose_triggered();
        ui->toolButton_user->setToolTip("登录");
        ui->toolButton_open->setToolTip("打开系统");
    }else{
        on_actionOpen_triggered();
        ui->toolButton_user->setToolTip("退出");
        ui->toolButton_open->setToolTip("关闭系统");
    }
    //isOpen = !isOpen;
}

void MainWindow::on_toolButton_user_clicked()
{
    if(isLonIn){
        on_actionExit_triggered();
        ui->toolButton_user->setToolTip("登录");
        ui->toolButton_open->setToolTip("打开系统");
    }else{
        on_actionLogn_triggered();
        ui->toolButton_user->setToolTip("退出");
        ui->toolButton_open->setToolTip("关闭系统");
    }
    //isLonIn = !isLonIn;
}
