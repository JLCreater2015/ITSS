#include "parkrundialog.h"
#include "ui_parkrundialog.h"
#include "variabledef.h"
#include <QMessageBox>

ParkRunDialog::ParkRunDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ParkRunDialog)
{
    ui->setupUi(this);
    init();
}

ParkRunDialog::~ParkRunDialog()
{
    if(db.isOpen())
        db.close();
    delete ui;
}

void ParkRunDialog::showEvent(QShowEvent *event)
{
    ui->label->setText("暂无动态");
}

void ParkRunDialog::showInfoIntime(QString m)
{
    QStringList myList;
    QDateTime current_date_time = QDateTime::currentDateTime();
    QString cdt = current_date_time.toString("MM-dd hh:mm:ss");
    QString cdt0 = current_date_time.toString("yyyy-MM-dd hh:mm:ss");
    myList.append(cdt0);//1.date
    myList.append(m.split("#").at(2));//2.cardID
    QSqlQuery query(this->db);
    query.prepare("SELECT userName,carNumber,carType from user_parking where cardID=?");
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
        QString site;
        query.prepare("SELECT leftSite from sites");
        if(!query.exec())
            return;
        while(query.next())
        {
            int fieldNo9 = query.record().indexOf("leftSite");
            site = query.value(fieldNo9).toString();
        }
        ui->label->setText("剩余"+site+"个车位");
        query.finish();
        ui->tableWidget->insertRow(rowCount);
        //坑：必须插入一行才能显示数字
        ui->tableWidget->setRowHeight(rowCount,25);
        ui->tableWidget->setItem(rowCount,0,new QTableWidgetItem(cdt));
        ui->tableWidget->setItem(rowCount,1,new QTableWidgetItem(list1.at(1)));
        ui->tableWidget->setItem(rowCount,2,new QTableWidgetItem(m.split("#").at(2)));
        ui->tableWidget->setItem(rowCount,3,new QTableWidgetItem("该用户进入停车场"));
        ui->tableWidget->show();
        rowCount++;
    }else{
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
        ui->label->setText("用户"+myList.at(3)+"收费"+myList.at(7)+"元");
        query.prepare("DELETE FROM enter_car WHERE carNumber=?");
        query.addBindValue(myList.at(3));
        query.exec();
        query.finish();
        ui->tableWidget->insertRow(rowCount);
        ui->tableWidget->setRowHeight(rowCount,25);
        ui->tableWidget->setItem(rowCount,0,new QTableWidgetItem(cdt));
        ui->tableWidget->setItem(rowCount,1,new QTableWidgetItem(list1.at(1)));
        ui->tableWidget->setItem(rowCount,2,new QTableWidgetItem(m.split("#").at(2)));
        ui->tableWidget->setItem(rowCount,3,new QTableWidgetItem("收费："+myList.at(7)));
        ui->tableWidget->show();
        rowCount++;
    }
}

void ParkRunDialog::init()
{
    ui->tableWidget->setColumnCount(4);
    ui->tableWidget->setRowCount(0);
    /* 设置 tableWidget */
    ui->tableWidget->setHorizontalHeaderLabels(QStringList() << "时间" << "车牌" << "标签" << "详情");
    //ui->tableWidget->verticalHeader()->setVisible(false); // 隐藏水平header
    ui->tableWidget->horizontalHeader()->setStyleSheet("QHeaderView::section {background-color:green;"
                                                           "color: black;font-size:14px;min-height: 28px;border: 2px inset #6c6c6c;}");
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);//只读
    ui->tableWidget->setColumnWidth(0,150);
    ui->tableWidget->setColumnWidth(1,95);
    ui->tableWidget->setColumnWidth(2,120);
    ui->tableWidget->setAlternatingRowColors(true);
    ui->tableWidget->setStyleSheet( "QTableView{background-color: rgb(250, 250, 115);"
                                  "alternate-background-color: rgb(141, 163, 215);}" );
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidget->horizontalHeader()->setHighlightSections(false);
    rowCount=ui->tableWidget->rowCount();
    ui->label->setText("暂无动态");

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
}

void ParkRunDialog::closeEvent(QCloseEvent *event)
{
    on_pushButtonEmpty_clicked();
    emit closeSign2(1);
}

void ParkRunDialog::on_pushButtonEmpty_clicked()
{
    ui->tableWidget->clearContents();
    rowCount=ui->tableWidget->rowCount();
    for(int j = rowCount-1;j >= 0;j--)
    {
        ui->tableWidget->removeRow(j);
    }
    ui->label->setText("智能停车场欢迎您！");
    rowCount = 0;
}
