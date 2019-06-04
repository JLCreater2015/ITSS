#include "parkcharge.h"
#include "ui_parkcharge.h"
#include <QtSql>
#include <QSqlQuery>
#include <QMessageBox>
#include "variabledef.h"
#include <QDate>
ParkCharge::ParkCharge(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ParkCharge)
{
    ui->setupUi(this);
    //准备数据模型
    if(QSqlDatabase::contains("myConnection_parkcharge"))
        db = QSqlDatabase::database("myConnection_parkcharge");
    else
        db = QSqlDatabase::addDatabase("QSQLITE","myConnection_parkcharge");//添加数据库驱动
    db.setDatabaseName(DBPath);
    if(!db.open()) //打开数据库
    {
        QMessageBox::critical(0, qApp->tr("Cannot open database"),
                              qApp->tr("Unable to establish a database connection. "),
                              QMessageBox::Cancel);
        return;
    }
    model = new QSqlQueryModel(ui->tableView);
}

ParkCharge::~ParkCharge()
{
    if(db.open())
        db.close();
    if(model)
        delete model;
    delete ui;
}

void ParkCharge::init(int v)
{
    if(v == 0)
        model->setQuery(QString("SELECT carNumber,startTime,endTime,payMoney,unitPrice from pay_records"),db);
    else
    {
        QString sqlstr = "SELECT carNumber,startTime,endTime,payMoney,unitPrice from pay_records WHERE carNumber='"
                +ui->textEditCar->toPlainText()+"' AND startTime BETWEEN datetime('"
                +ui->dateEditStart->dateTime().toString("yyyy-MM-dd hh:mm:ss")
                +"') AND datetime('"+ui->dateEditStop->dateTime().toString("yyyy-MM-dd hh:mm:ss")+"')";
        model->setQuery(sqlstr,db);
    }
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("车牌"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("进车时间"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("出车时间"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("收费金额"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("单价（元/小时）"));
    ui->tableView->horizontalHeader()->setStyleSheet("QHeaderView::section {background-color:green;"
                                                           "color: black;font-size:16px;min-height: 28px;border: 2px inset #6c6c6c;}");

    ui->tableView->setAlternatingRowColors(true);
    ui->tableView->setStyleSheet( "QTableView{background-color: rgb(250, 250, 115);"
                                  "alternate-background-color: rgb(141, 163, 215);}" );
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->horizontalHeader()->setStretchLastSection(true);
    ui->tableView->horizontalHeader()->setHighlightSections(false);
    //ui->tableView->verticalHeader()->setVisible(false);
    //ui->tableView->setShowGrid(false);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView->setSelectionMode(QAbstractItemView::ExtendedSelection);

    ui->tableView->setModel(model);
    ui->tableView->setColumnWidth(0,120);
    ui->tableView->setColumnWidth(1,180);
    ui->tableView->setColumnWidth(2,180);
    ui->tableView->setColumnWidth(3,90);
    //坑：在进行表格设置时必须是“ui->tableView->setModel(model);”在前，属性具体设置在后，反之则设置不会生效。
    ui->tableView->show();

    ui->dateEditStart->setDate(QDate::fromString("20130101", "yyyyMMdd"));
    ui->dateEditStop->setDate(QDate::currentDate());
    ui->dateEditStart->setTime(QTime::fromString("00:00:00","hh:mm:ss"));
    ui->dateEditStop->setTime(QTime::fromString("23:59:59","hh:mm:ss"));
    ui->pushButtonQuery->setEnabled(false);
}

void ParkCharge::on_toolButton_clicked()
{
    model->clear();
    init(0);
}
void ParkCharge::showEvent(QShowEvent *event)
{
    model->clear();
    init(0);
}

void ParkCharge::on_textEditCar_cursorPositionChanged()
{
    if(ui->textEditCar->toPlainText() != "")
        ui->pushButtonQuery->setEnabled(true);
    else
        ui->pushButtonQuery->setEnabled(false);
}

void ParkCharge::on_dateEditStop_userDateChanged(const QDate &d)
{
    if(d > QDate::currentDate())
        ui->dateEditStart->setDate(QDate::currentDate());
}

void ParkCharge::on_dateEditStart_userDateChanged(const QDate &d)
{
    if(d > QDate::currentDate())
        ui->dateEditStop->setDate(QDate::currentDate());
}

void ParkCharge::on_pushButtonQuery_clicked()
{
    model->clear();
    init(1);
}

void ParkCharge::on_dateEditStart_dateChanged(const QDate &d)
{
    if(d > QDate::currentDate())
        ui->dateEditStart->setDate(QDate::currentDate());
}

void ParkCharge::on_dateEditStart_timeChanged(const QTime &t)
{
    ui->dateEditStart->setTime(QTime::fromString("00:00:00","hh:mm:ss"));
}

void ParkCharge::on_dateEditStop_dateChanged(const QDate &d)
{
    if(d > QDate::currentDate())
        ui->dateEditStop->setDate(QDate::currentDate());
}

void ParkCharge::on_dateEditStop_timeChanged(const QTime &t)
{
    ui->dateEditStop->setTime(QTime::fromString("23:59:59","hh:mm:ss"));
}
