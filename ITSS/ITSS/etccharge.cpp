#include "etccharge.h"
#include "ui_etccharge.h"
#include "variabledef.h"
#include <QtSql>
#include <QSqlQuery>
#include <QMessageBox>
#include <QDate>
ETCcharge::ETCcharge(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ETCcharge)
{
    ui->setupUi(this);
    //准备数据模型QSqlDatabase db;

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
    model = new QSqlQueryModel(ui->tableView);
}

ETCcharge::~ETCcharge()
{
    if(db.open())
        db.close();
    if(model)
        delete model;
    delete ui;
}

void ETCcharge::init(int v)
{
    if(v == 0)
        model->setQuery(QString("SELECT name,carNum,tagID,amount,recordType,date from records"),db);
    else
    {
        QString sqlstr = "SELECT name,carNum,tagID,amount,recordType,date from records WHERE carNum='"
                +ui->textEditCar->toPlainText()+"' AND date BETWEEN datetime('"
                +ui->dateEditStart->dateTime().toString("yyyy-MM-dd hh:mm:ss")
                +"') AND datetime('"+ui->dateEditStop->dateTime().toString("yyyy-MM-dd hh:mm:ss")+"')";
        model->setQuery(sqlstr,db);
    }
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("车主"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("车牌"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("标签号"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("金额"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("收费/充值"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("时间"));
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
    ui->tableView->setColumnWidth(0,75);
    ui->tableView->setColumnWidth(1,95);
    ui->tableView->setColumnWidth(2,180);
    ui->tableView->setColumnWidth(3,80);
    ui->tableView->setColumnWidth(4,100);
    //坑：在进行表格设置时必须是“ui->tableView->setModel(model);”在前，属性具体设置在后，反之则设置不会生效。
    ui->tableView->show();

    ui->dateEditStart->setDate(QDate::fromString("20130101", "yyyyMMdd"));
    ui->dateEditStop->setDate(QDate::currentDate());
    ui->dateEditStart->setTime(QTime::fromString("00:00:00","hh:mm:ss"));
    ui->dateEditStop->setTime(QTime::fromString("23:59:59","hh:mm:ss"));
    ui->pushButtonQuery->setEnabled(false);
}

void ETCcharge::on_toolButton_clicked()
{
    model->clear();
    init(0);
}

void ETCcharge::showEvent(QShowEvent *event)
{
    model->clear();
    init(0);
}

void ETCcharge::on_textEditCar_cursorPositionChanged()
{
    if(ui->textEditCar->toPlainText() != "")
        ui->pushButtonQuery->setEnabled(true);
    else
        ui->pushButtonQuery->setEnabled(false);
}

void ETCcharge::on_dateEditStart_userDateChanged(const QDate &d)
{
    if(d > QDate::currentDate())
        ui->dateEditStart->setDate(QDate::currentDate());
}

void ETCcharge::on_dateEditStop_userDateChanged(const QDate &d)
{
    if(d > QDate::currentDate())
        ui->dateEditStop->setDate(QDate::currentDate());
}

void ETCcharge::on_pushButtonQuery_clicked()
{
    model->clear();
    init(1);
}

void ETCcharge::on_dateEditStart_dateChanged(const QDate &d)
{
    if(d > QDate::currentDate())
        ui->dateEditStart->setDate(QDate::currentDate());
}

void ETCcharge::on_dateEditStart_timeChanged(const QTime &t)
{
    ui->dateEditStart->setTime(QTime::fromString("00:00:00","hh:mm:ss"));
}

void ETCcharge::on_dateEditStop_dateChanged(const QDate &d)
{
    if(d > QDate::currentDate())
        ui->dateEditStop->setDate(QDate::currentDate());
}

void ETCcharge::on_dateEditStop_timeChanged(const QTime &t)
{
    ui->dateEditStop->setTime(QTime::fromString("23:59:59","hh:mm:ss"));
}
