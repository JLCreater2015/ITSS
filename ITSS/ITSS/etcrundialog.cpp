#include "etcrundialog.h"
#include "ui_etcrundialog.h"
#include <QMessageBox>
#include "variabledef.h"

ETCRunDialog::ETCRunDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ETCRunDialog)
{
    ui->setupUi(this);
    init();
}

ETCRunDialog::~ETCRunDialog()
{
    if(db.isOpen())
        db.close();
    delete ui;
}

void ETCRunDialog::showEvent(QShowEvent *event)
{
    ui->label->setText("Hello");
}

void ETCRunDialog::init()
{
    ui->tableWidget->setColumnCount(4);
    ui->tableWidget->setRowCount(0);
    /* 设置 tableWidget */
    ui->tableWidget->setHorizontalHeaderLabels(QStringList() << "时间" << "车牌" << "标签" << "详情");
    //ui->tableWidget->verticalHeader()->setVisible(false); // 隐藏水平header
    ui->tableWidget->horizontalHeader()->setStyleSheet("QHeaderView::section {background-color:green;"
                                                           "color: black;font-size:14px;min-height: 28px;border: 2px inset #6c6c6c;}");
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);//只读
    ui->tableWidget->setColumnWidth(0,160);
    ui->tableWidget->setColumnWidth(1,100);
    ui->tableWidget->setColumnWidth(2,120);
    ui->tableWidget->setAlternatingRowColors(true);
    ui->tableWidget->setStyleSheet( "QTableView{background-color: rgb(250, 250, 115);"
                                  "alternate-background-color: rgb(141, 163, 215);}" );
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidget->horizontalHeader()->setHighlightSections(false);
    //ui->tableWidget->setShowGrid(false);
    //ui->tableWidget->setSelectionMode(QAbstractItemView::ExtendedSelection);
    //ui->tableWidget->setRowCount(0);
    rowCount=ui->tableWidget->rowCount();
    /*
    ui->tableWidget->insertRow(rowCount);
    ///坑：必须插入一行才能显示数字
    ui->tableWidget->setRowHeight(rowCount,30);
    ui->tableWidget->setItem(rowCount,0,new QTableWidgetItem("aaaa"));
    ui->tableWidget->setItem(rowCount,1,new QTableWidgetItem("bbbb"));
    ui->tableWidget->setItem(rowCount,2,new QTableWidgetItem("cccc"));

    ui->tableWidget->setItem(rowCount,3,new QTableWidgetItem("dddd"));
    ui->tableWidget->show();
    rowCount++;
    */
    ui->label->setText("暂无动态");

    if(QSqlDatabase::contains("myConnection_etcdialog"))
        db = QSqlDatabase::database("myConnection_etcdialog");
    else
        db = QSqlDatabase::addDatabase("QSQLITE","myConnection_etcdialog");//添加数据库驱动
    db.setDatabaseName(DBPath);
    if(!db.open()) //打开数据库
    {
        QMessageBox::critical(0, qApp->tr("Cannot open database"),
                              qApp->tr("Unable to establish a database connection. "),
                              QMessageBox::Cancel);
        return;
    }
}

void ETCRunDialog::showInfoIntime(QString m)
{
    QDateTime current_date_time = QDateTime::currentDateTime();
    QString cdt = current_date_time.toString("MM-dd hh:mm:ss");

    QSqlQuery query(this->db);
    query.prepare("SELECT name,carNum,carType from users where tagID=?");
    query.addBindValue(m.split("#").at(1));
    if(!query.exec())
    {
        return;
    }
    QStringList list1;
    while(query.next())
    {
        int fieldNo1 = query.record().indexOf("carNum");
        int fieldNo2 = query.record().indexOf("carType");
        int fieldNo3 = query.record().indexOf("name");
        QString v1 = query.value(fieldNo1).toString();
        QString v2 = query.value(fieldNo2).toString();
        QString v3 = query.value(fieldNo3).toString();
        list1.append(v1);
        list1.append(v2);
        list1.append(v3);
    }
    if(list1.isEmpty() || list1.length() < 3)
    {
        return;
    }
    query.prepare("SELECT price from cartypes where cartype=?");
    query.addBindValue(list1.at(1));
    if(!query.exec())
    {
        return;
    }
    while(query.next())
    {
        int fieldNo4 = query.record().indexOf("price");
        QString v4 = query.value(fieldNo4).toString();
        list1.append(v4);
    }
    query.finish();
    if(list1.isEmpty() || list1.length() < 4)
    {
        return;
    }
    ui->tableWidget->insertRow(rowCount);
    //坑：必须插入一行才能显示数字
    ui->tableWidget->setRowHeight(rowCount,25);
    ui->tableWidget->setItem(rowCount,0,new QTableWidgetItem(cdt));
    ui->tableWidget->setItem(rowCount,1,new QTableWidgetItem(list1.first()));
    ui->tableWidget->setItem(rowCount,2,new QTableWidgetItem(m.split("#").at(1)));

    ui->tableWidget->setItem(rowCount,3,new QTableWidgetItem("消费"+list1.at(3)+"元"));
    ui->tableWidget->show();
    rowCount++;

    ui->label->setText(QString::number(rowCount, 10));
}

void ETCRunDialog::closeEvent(QCloseEvent *event)
{
    on_pushButtonEmpty_clicked();
    emit closeSign1(0);
}

void ETCRunDialog::on_pushButtonEmpty_clicked()
{
    ui->tableWidget->clearContents();
    rowCount=ui->tableWidget->rowCount();
    for(int j = rowCount-1;j >= 0;j--){
        ui->tableWidget->removeRow(j);
    }
    rowCount = 0;
    ui->label->setText("0");
}

