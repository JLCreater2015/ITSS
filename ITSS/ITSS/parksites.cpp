#include "parksites.h"
#include "ui_parksites.h"
#include <QtSql>
#include "variabledef.h"
#include <QSqlQuery>
#include <QMessageBox>
#include <QInputDialog>
#include "MyUtils/loginusrsingleton.h"

ParkSites::ParkSites(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ParkSites)
{
    ui->setupUi(this);
    //准备数据模型
    if(QSqlDatabase::contains("myConnection_parksite"))
        db = QSqlDatabase::database("myConnection_parksite");
    else
        db = QSqlDatabase::addDatabase("QSQLITE","myConnection_parksite");//添加数据库驱动
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

ParkSites::~ParkSites()
{
    if(db.open())
        db.close();
    if(model)
        delete model;
    delete ui;
}

void ParkSites::init(int v)
{

    model->setQuery(QString("SELECT userName,carNumber,cardBalance,userPhone,registerTime from user_parking"),db);
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("车主"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("车牌"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("余额"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("车主电话"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("注册时间"));
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
    ui->tableView->setColumnWidth(1,140);
    ui->tableView->setColumnWidth(2,130);
    ui->tableView->setColumnWidth(3,140);
    //坑：在进行表格设置时必须是“ui->tableView->setModel(model);”在前，属性具体设置在后，反之则设置不会生效。
    ui->tableView->show();
    QSqlQuery query(this->db);
    query.prepare("SELECT maxSite,leftSite from sites");
    if(!query.exec())
    {
        return;
    }
    if(query.next())
    {
        int fieldNo1 = query.record().indexOf("maxSite");
        int fieldNo2 = query.record().indexOf("leftSite");
        QString v1 = query.value(fieldNo1).toString();
        QString v2 = query.value(fieldNo2).toString();
        ui->lineEditAllCount->setText(v1);
        ui->lineEditRemainCount->setText(v2);
        bool ok;
        int a = v1.toInt(&ok,10) - v2.toInt(&ok,10);
        ui->lineEditUsedCount->setText(QString::number(a,10));
    }
    else
    {
        ui->lineEditAllCount->setText("0");
        ui->lineEditRemainCount->setText("0");
        ui->lineEditUsedCount->setText("0");
    }
    query.finish();
    ui->pushButtonQuery->setEnabled(false);
    ui->pushButtonRecharge->setEnabled(false);
}

void ParkSites::showEvent(QShowEvent *event)
{
    model->clear();
    init(0);
    LogInUsrSingleton *pSingleton1 = LogInUsrSingleton::getInstance();
    if(pSingleton1->getauthority() == 2)
    {
        ui->pushButtonReset->setEnabled(false);
        ui->textEditMoney->setEnabled(false);
    }
}

void ParkSites::on_textEditCar_cursorPositionChanged()
{
    if(ui->textEditCar->toPlainText() != "")
        ui->pushButtonQuery->setEnabled(true);
    else
        ui->pushButtonQuery->setEnabled(false);
}

void ParkSites::on_textEditMoney_cursorPositionChanged()
{
    int row= ui->tableView->currentIndex().row();
    if(ui->textEditMoney->toPlainText() != "" && (ui->textEditCar->toPlainText() != "" || row >= 0))
        ui->pushButtonRecharge->setEnabled(true);
    else
        ui->pushButtonRecharge->setEnabled(false);
}

void ParkSites::on_pushButtonReset_clicked()
{
    bool ok;
    int initVal = ui->lineEditAllCount->text().toInt(&ok,10);
    unsigned int interval;
    interval = QInputDialog::getInt(this, tr("Reset:"),
                                        tr("Reset max count of site:"), initVal, initVal, BigInt, 1, &ok);
    if(ok){
        QSqlQuery query(this->db);
        query.prepare("UPDATE sites SET maxSite=?");
        query.addBindValue(QString::number(interval, 10));
        if(query.exec())
        {
            QMessageBox::information(this, tr("提示"),tr("重置成功！"));
            ui->lineEditAllCount->setText(QString::number(interval, 10));
            ui->lineEditUsedCount->setText(QString::number(interval-ui->lineEditRemainCount->text().toInt(&ok,10), 10));
        }
        else
            QMessageBox::information(this, tr("提示"),tr("重置失败..."));
        query.finish();
    }
}

void ParkSites::on_pushButtonRecharge_clicked()
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
        query.prepare(QString("SELECT cardBalance FROM user_parking WHERE carNumber=?"));
        query.addBindValue(carNumber);
        if(query.exec())
        {
            if(query.next())
            {
                re = query.value(0).toString().toInt(&ok,10) + ui->textEditMoney->toPlainText().toInt(&ok,10);
                query.prepare(QString("UPDATE user_parking SET cardBalance=? WHERE carNumber=?"));
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

void ParkSites::on_pushButtonQuery_clicked()
{
    model->clear();
    init(1);
    ui->pushButtonQuery->setEnabled(false);
}

void ParkSites::on_toolButton_clicked()
{
    model->clear();
    init(0);
}

void ParkSites::on_tableView_doubleClicked(const QModelIndex &index)
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
    query.prepare(QString("SELECT cardBalance FROM user_parking WHERE carNumber=?"));
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
                query.prepare(QString("UPDATE user_parking SET cardBalance=? WHERE carNumber=?"));
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
