#include "trafficviolationrecord.h"
#include "ui_trafficviolationrecord.h"
#include "variabledef.h"
#include <QtSql>
#include <QSqlQuery>
#include <QMessageBox>
#include <QDate>
TrafficViolationRecord::TrafficViolationRecord(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TrafficViolationRecord)
{
    ui->setupUi(this);
    if(QSqlDatabase::contains("myConnection_etccharge"))
        db = QSqlDatabase::database("myConnection_etccharge");
    else
        db = QSqlDatabase::addDatabase("QSQLITE","myConnection_etccharge");//添加数据库驱动
    db.setDatabaseName(DBPath);
    if(!db.open()) //打开数据库
    {
        QMessageBox::critical(0, qApp->tr("Cannot open database"),
                              qApp->tr("Unable to establish a database connection. "),
                              QMessageBox::Cancel);
        return;
    }
    model1 = new QSqlQueryModel(ui->tableView_1);
    model2 = new QSqlQueryModel(ui->tableView_2);
}

TrafficViolationRecord::~TrafficViolationRecord()
{
    if(db.open())
        db.close();
    if(model1)
        delete model1;
    if(model2)
        delete model2;
    delete ui;
}

void TrafficViolationRecord::init(int v)
{
    if(v == 0)
    {
        model1->setQuery(QString("SELECT owner_name,carNum,photographTime FROM car_distribute where rowid%2=1"),db);
        model2->setQuery(QString("SELECT owner_name,carNum,photographTime FROM car_distribute where rowid%2=0"),db);
    }
    else
    {
        QString sqlstr1 = "SELECT owner_name,carNum,photographTime FROM car_distribute WHERE carNum='"
                +ui->textEditCar->toPlainText()+"'";
        QString sqlstr2 = "SELECT owner_name,carNum,photographTime FROM car_distribute WHERE carNum='"
                +ui->textEditCar->toPlainText()+"' AND photographTime BETWEEN datetime('"
                +ui->dateEditStart->dateTime().toString("yyyy-MM-dd hh:mm:ss")
                +"') AND datetime('"+ui->dateEditStop->dateTime().toString("yyyy-MM-dd hh:mm:ss")+"')";
        model1->setQuery(sqlstr1,db);
        model2->setQuery(sqlstr2,db);
    }
    model1->setHeaderData(0, Qt::Horizontal, QObject::tr("车主"));
    model1->setHeaderData(1, Qt::Horizontal, QObject::tr("车牌"));
    model1->setHeaderData(5, Qt::Horizontal, QObject::tr("违章时间"));
    ui->tableView_1->horizontalHeader()->setStyleSheet("QHeaderView::section {background-color:green;"
                                                           "color: black;font-size:16px;min-height: 28px;border: 2px inset #6c6c6c;}");
    ui->tableView_1->setAlternatingRowColors(true);
    ui->tableView_1->setStyleSheet( "QTableView{background-color: rgb(250, 250, 115);"
                                  "alternate-background-color: rgb(141, 163, 215);}" );
    ui->tableView_1->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView_1->horizontalHeader()->setStretchLastSection(true);
    ui->tableView_1->horizontalHeader()->setHighlightSections(false);
    ui->tableView_1->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView_1->setSelectionMode(QAbstractItemView::ExtendedSelection);
    ui->tableView_1->setModel(model1);
    ui->tableView_1->setColumnWidth(0,70);
    ui->tableView_1->setColumnWidth(1,90);
    ui->tableView_1->setColumnWidth(2,180);
    ui->tableView_1->show();

    model2->setHeaderData(0, Qt::Horizontal, QObject::tr("车主"));
    model2->setHeaderData(1, Qt::Horizontal, QObject::tr("车牌"));
    model2->setHeaderData(5, Qt::Horizontal, QObject::tr("违章时间"));
    ui->tableView_2->horizontalHeader()->setStyleSheet("QHeaderView::section {background-color:green;"
                                                           "color: black;font-size:16px;min-height: 28px;border: 2px inset #6c6c6c;}");
    ui->tableView_2->setAlternatingRowColors(true);
    ui->tableView_2->setStyleSheet( "QTableView{background-color: rgb(250, 250, 115);"
                                  "alternate-background-color: rgb(141, 163, 215);}" );
    ui->tableView_2->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView_2->horizontalHeader()->setStretchLastSection(true);
    ui->tableView_2->horizontalHeader()->setHighlightSections(false);
    ui->tableView_2->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView_2->setSelectionMode(QAbstractItemView::ExtendedSelection);
    ui->tableView_2->setModel(model2);
    ui->tableView_2->setColumnWidth(0,70);
    ui->tableView_2->setColumnWidth(1,90);
    ui->tableView_2->setColumnWidth(2,180);
    ui->tableView_2->show();

    ui->dateEditStart->setDate(QDate::fromString("20130101", "yyyyMMdd"));
    ui->dateEditStop->setDate(QDate::currentDate());
    ui->dateEditStart->setTime(QTime::fromString("00:00:00","hh:mm:ss"));
    ui->dateEditStop->setTime(QTime::fromString("23:59:59","hh:mm:ss"));
    ui->pushButtonQuery->setEnabled(false);
}

void TrafficViolationRecord::showEvent(QShowEvent *event)
{
    model1->clear();
    model2->clear();
    init(0);
}

void TrafficViolationRecord::on_toolButton_clicked()
{
    model1->clear();
    model2->clear();
    init(0);
}

void TrafficViolationRecord::on_pushButtonQuery_clicked()
{
    model1->clear();
    model2->clear();
    init(1);
}

void TrafficViolationRecord::on_textEditCar_textChanged()
{
    if(ui->textEditCar->toPlainText() != "")
        ui->pushButtonQuery->setEnabled(true);
    else
        ui->pushButtonQuery->setEnabled(false);
}

void TrafficViolationRecord::on_dateEditStart_dateChanged(const QDate &d)
{
    if(d > QDate::currentDate())
        ui->dateEditStart->setDate(QDate::currentDate());
}

void TrafficViolationRecord::on_dateEditStop_dateChanged(const QDate &d)
{
    if(d > QDate::currentDate())
        ui->dateEditStop->setDate(QDate::currentDate());
}

void TrafficViolationRecord::on_dateEditStart_timeChanged(const QTime &t)
{
    ui->dateEditStart->setTime(QTime::fromString("00:00:00","hh:mm:ss"));
}

void TrafficViolationRecord::on_dateEditStop_timeChanged(const QTime &t)
{
    ui->dateEditStop->setTime(QTime::fromString("23:59:59","hh:mm:ss"));
}
