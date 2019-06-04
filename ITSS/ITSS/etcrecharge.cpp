#include "etcrecharge.h"
#include "ui_etcrecharge.h"
#include <QtSql>
#include "variabledef.h"
#include <QSqlQuery>
#include <QMessageBox>
#include <QInputDialog>
#include "MyUtils/loginusrsingleton.h"
EtcRecharge::EtcRecharge(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EtcRecharge)
{
    ui->setupUi(this);
    //准备数据模型
    if(QSqlDatabase::contains("myConnection_etcrecharge"))
        db = QSqlDatabase::database("myConnection_etcrecharge");
    else
        db = QSqlDatabase::addDatabase("QSQLITE","myConnection_etcrecharge");//添加数据库驱动
    db.setDatabaseName(DBPath);
    if(!db.open()) //打开数据库
    {
        QMessageBox::critical(0, qApp->tr("Cannot open database"),
                              qApp->tr("Unable to establish a database connection. "),
                              QMessageBox::Cancel);
        return;
    }
    model = new QSqlQueryModel(ui->tableView);
    //init();
}

EtcRecharge::~EtcRecharge()
{
    if(db.open())
        db.close();
    if(model)
        delete model;
    delete ui;
}

void EtcRecharge::init(int v)
{
    if(v == 0)
        model->setQuery(QString("SELECT name,carNum,tagID,carType,balance,tel from users"),db);
    else
        model->setQuery(QString("SELECT name,carNum,tagID,carType,balance,tel from users WHERE carNum='"
                                + ui->textEditCar->toPlainText() + "'"),db);
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("车主"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("车牌"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("标签号"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("车型"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("余额"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("车主电话"));
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
    ui->tableView->setColumnWidth(0,90);
    ui->tableView->setColumnWidth(1,140);
    ui->tableView->setColumnWidth(2,150);
    ui->tableView->setColumnWidth(3,90);
    ui->tableView->setColumnWidth(4,80);
    //坑：在进行表格设置时必须是“ui->tableView->setModel(model);”在前，属性具体设置在后，反之则设置不会生效。
    ui->tableView->show();
    ui->pushButtonQuery->setEnabled(false);
    ui->pushButtonRecharge->setEnabled(false);
}

void EtcRecharge::on_toolButton_clicked()
{
    model->clear();
    init(0);
}
void EtcRecharge::showEvent(QShowEvent *event)
{
    model->clear();
    init(0);
    LogInUsrSingleton *pSingleton1 = LogInUsrSingleton::getInstance();
    if(pSingleton1->getauthority() == 2)
    {
        ui->textEditMoney->setEnabled(false);
    }
}

void EtcRecharge::on_pushButtonRecharge_clicked()
{
    int row= ui->tableView->currentIndex().row();
    if(ui->textEditCar->toPlainText() != "" || row >= 0)
    {
        QSqlQuery query(db);
        int re = 0;
        bool ok;
        QString carNumber;
        if(ui->textEditCar->toPlainText() != "")
            carNumber = ui->textEditCar->toPlainText();
        else
        {
            QAbstractItemModel *mod = ui->tableView->model();
            QModelIndex index = mod->index(row,1);
            QVariant data = mod->data(index);
            carNumber = data.toString();
        }
        query.prepare(QString("SELECT balance FROM users WHERE carNum=?"));
        query.addBindValue(carNumber);
        if(query.exec())
        {
            if(query.next())
            {
                re = query.value(0).toString().toInt(&ok,10) + ui->textEditMoney->toPlainText().toInt(&ok,10);
                query.prepare(QString("UPDATE users SET balance=? WHERE carNum=?"));
                query.addBindValue(QString::number(re, 10));
                query.addBindValue(carNumber);
                if(query.exec())
                {
                    QMessageBox::information(this, tr("提示"),tr("充值成功！"));
                    on_toolButton_clicked();
                }
                else
                    QMessageBox::information(this, tr("提示"),tr("充值失败..."));
            }
            else
            {
                QMessageBox::information(this, tr("提示"),tr("信息不存在..."));
            }
        }
        else
        {
            QMessageBox::information(this, tr("提示"),tr("信息查询失败..."));
        }
        query.finish();
    }
    else
        QMessageBox::information(this, tr("提示"),tr("无效操作！"));
    ui->pushButtonRecharge->setEnabled(false);
}

void EtcRecharge::on_textEditCar_cursorPositionChanged()
{
    if(ui->textEditCar->toPlainText() != "")
        ui->pushButtonQuery->setEnabled(true);
    else
        ui->pushButtonQuery->setEnabled(false);

}

void EtcRecharge::on_textEditMoney_cursorPositionChanged()
{
    int row= ui->tableView->currentIndex().row();
    if(ui->textEditMoney->toPlainText() != "" && (ui->textEditCar->toPlainText() != "" || row >= 0))
        ui->pushButtonRecharge->setEnabled(true);
    else
        ui->pushButtonRecharge->setEnabled(false);
}

void EtcRecharge::on_pushButtonQuery_clicked()
{
    model->clear();
    init(1);
    ui->pushButtonQuery->setEnabled(false);
}

void EtcRecharge::on_tableView_doubleClicked(const QModelIndex &index)
{
    int row= index.row();
    if(row < 0)
        return;
    QAbstractItemModel *mod = ui->tableView->model();
    QModelIndex index0 = mod->index(row,1);//选中行第二列的内容
    QVariant data = mod->data(index0);
    QSqlQuery query(db);
    int re = 0;
    bool ok;
    QString carNumber = data.toString();
    query.prepare(QString("SELECT balance FROM users WHERE carNum=?"));
    query.addBindValue(carNumber);
    if(query.exec())
    {
        if(query.next())
        {
            unsigned int interval;
            interval = QInputDialog::getInt(this, tr("Recharge:"),
                                                tr("amount:(>0)"), 0, 0, BigInt, 1, &ok);
            if(ok){
                re = query.value(0).toString().toInt(&ok,10) + interval;
                query.prepare(QString("UPDATE users SET balance=? WHERE carNum=?"));
                query.addBindValue(QString::number(re, 10));
                query.addBindValue(carNumber);
                if(query.exec())
                {
                    on_toolButton_clicked();
                    QMessageBox::information(this, tr("提示"),tr("充值成功！"));
                }
                else
                    QMessageBox::information(this, tr("提示"),tr("充值失败..."));
            }
            else
            {
                QMessageBox::information(this, tr("提示"),tr("取消操作！"));
            }
        }
        else
        {
            QMessageBox::information(this, tr("提示"),tr("信息不存在..."));
        }
    }
    else
    {
        QMessageBox::information(this, tr("提示"),tr("信息查询失败..."));
    }
    query.finish();
}
