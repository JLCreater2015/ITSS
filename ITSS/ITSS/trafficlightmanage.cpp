#include "trafficlightmanage.h"
#include "ui_trafficlightmanage.h"
#include "variabledef.h"
#include <QtSql>
#include <QSqlQuery>
#include <QMessageBox>
#include "MyUtils/loginusrsingleton.h"

TrafficLightManage::TrafficLightManage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TrafficLightManage)
{
    ui->setupUi(this);
    if(QSqlDatabase::contains("myConnection_lightinfo"))
        db = QSqlDatabase::database("myConnection_lightinfo");
    else
        db = QSqlDatabase::addDatabase("QSQLITE","myConnection_lightinfo");//添加数据库驱动
    db.setDatabaseName(DBPath);
    if(!db.open())
    {
        QMessageBox::critical(0, qApp->tr("Cannot open database"),
                              qApp->tr("Unable to establish a database connection. "),
                              QMessageBox::Cancel);
        return;
    }
    model = new QSqlQueryModel(ui->tableView);
    QObject::connect(&lightInfoAddDialog,SIGNAL(closeSign(int)),this,SLOT(sendOpenSignals(int)));
}

void TrafficLightManage::init(int v)
{
    if(v == 0)
    {
        model->setQuery(QString("SELECT light_number,record_type,record_time from light_info WHERE type='"+QString::number(sign, 10)+"'"),db);

    }else{
        model->setQuery(QString("SELECT light_number,record_type,record_time from light_info WHERE type='"+QString::number(sign, 10)+"' AND light_number='"+ui->textEdit->toPlainText()+"'"),db);
    }
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("编号"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("记录类型"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("记录时间"));
    ui->tableView->horizontalHeader()->setStyleSheet("QHeaderView::section {background-color:green;"
                                                           "color: black;font-size:16px;min-height: 28px;border: 2px inset #6c6c6c;}");
    ui->tableView->setAlternatingRowColors(true);
    ui->tableView->setStyleSheet( "QTableView{background-color: rgb(250, 250, 115);"
                                  "alternate-background-color: rgb(141, 163, 215);}" );
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->horizontalHeader()->setStretchLastSection(true);
    ui->tableView->horizontalHeader()->setHighlightSections(false);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView->setSelectionMode(QAbstractItemView::ExtendedSelection);
    ui->tableView->setModel(model);
    ui->tableView->setColumnWidth(0,200);
    ui->tableView->setColumnWidth(1,160);
    //坑：在进行表格设置时必须是“ui->tableView->setModel(model);”在前，属性具体设置在后，反之则设置不会生效。
    ui->tableView->show();
    ui->pushButtonSelect->setEnabled(false);
}

TrafficLightManage::~TrafficLightManage()
{
    if(db.open())
        db.close();
    if(model)
        delete model;
    delete ui;
}

void TrafficLightManage::sendOpenSignals(int v)
{
    if(v == 1)
        QMessageBox::information(this, tr("提示"),tr("操作成功！"));
    else if(v == 2)
        QMessageBox::information(this, tr("提示"),tr("操作失败！"));
    else
        QMessageBox::information(this, tr("提示"),tr("取消操作。"));
    model->clear();
    init(0);
}

void TrafficLightManage::setLightType(int v)
{
    sign = v;
}

void TrafficLightManage::showEvent(QShowEvent *event)
{
    if(sign == 0)
        ui->label->setText("交通灯编号");
    else
        ui->label->setText("建筑灯编号");
    QObject::connect(this,SIGNAL(carNumber(const QString,const QString,const QString,int)),
                     &lightInfoAddDialog,SLOT(setDatas(const QString,const QString,const QString,int)));
    model->clear();
    init(0);

    LogInUsrSingleton *pSingleton1 = LogInUsrSingleton::getInstance();
    if(pSingleton1->getauthority() == 2)
    {
        ui->pushButtonAdd->setEnabled(false);
        ui->pushButtonDelete->setEnabled(false);
        ui->pushButtonUpdate->setEnabled(false);
    }
}
//添加
void TrafficLightManage::on_pushButtonAdd_clicked()
{
    emit carNumber("","","",sign);
    lightInfoAddDialog.show();
}
//删除
void TrafficLightManage::on_pushButtonDelete_clicked()
{
    on_tableView_doubleClicked(ui->tableView->currentIndex());
}
//修改
void TrafficLightManage::on_pushButtonUpdate_clicked()
{
    int row= ui->tableView->currentIndex().row();
    if(row < 0)
        return;
    QAbstractItemModel *mod = ui->tableView->model();
    QModelIndex index1 = mod->index(row,0);
    QVariant data1 = mod->data(index1);
    QModelIndex index2 = mod->index(row,1);
    QVariant data2 = mod->data(index2);
    QModelIndex index3 = mod->index(row,2);
    QVariant data3 = mod->data(index3);
    emit carNumber(data1.toString(),data2.toString(),data3.toString(),sign);
    lightInfoAddDialog.show();
}

void TrafficLightManage::on_pushButtonSelect_clicked()
{
    model->clear();
    init(1);
    ui->pushButtonSelect->setEnabled(false);
}
//刷新
void TrafficLightManage::on_toolButton_clicked()
{
    model->clear();
    init(0);
}
//有编辑内容
void TrafficLightManage::on_textEdit_textChanged()
{
    if(ui->textEdit->toPlainText() != "")
        ui->pushButtonSelect->setEnabled(true);
    else
        ui->pushButtonSelect->setEnabled(false);
}
//双击一行删除
void TrafficLightManage::on_tableView_doubleClicked(const QModelIndex &index)
{    
    LogInUsrSingleton *pSingleton1 = LogInUsrSingleton::getInstance();
    if(pSingleton1->getauthority() == 2)
        return;

    int row= index.row();
    if(row < 0)
        return;
    QAbstractItemModel *mod = ui->tableView->model();
    QModelIndex index0 = mod->index(row,0);//选中行第二列的内容
    QModelIndex index1 = mod->index(row,2);
    QVariant data = mod->data(index0);
    QVariant data_time = mod->data(index1);
    ///---实例化消息对话框对象
    QMessageBox *msgBox;
    if(sign == 0)
    {
        msgBox = new QMessageBox(tr("Confirm"),       ///--这里是设置消息框标题
            "确认删除编号号为 " + data.toString() + " 时间为 "+ data_time.toString() +" 的交通灯信息?",                       ///--这里是设置消息框显示的内容
            QMessageBox::Warning,                          ///--这里是在消息框显示的图标
            QMessageBox::Yes | QMessageBox::Default,     ///---这里是显示消息框上的按钮情况
            QMessageBox::Cancel | QMessageBox::Escape,  ///---这里与 键盘上的 escape 键结合。当用户按下该键，消息框将执行cancel按钮事件
            0);                                                     ///---这里是 定义第三个按钮， 该例子 只是 了显示2个按钮
    }else{
        msgBox = new QMessageBox(tr("Confirm"),       ///--这里是设置消息框标题
            "确认删除编号号为 " + data.toString() + " 时间为 "+ data_time.toString() +" 的建筑灯信息?",                       ///--这里是设置消息框显示的内容
            QMessageBox::Warning,                          ///--这里是在消息框显示的图标
            QMessageBox::Yes | QMessageBox::Default,     ///---这里是显示消息框上的按钮情况
            QMessageBox::Cancel | QMessageBox::Escape,  ///---这里与 键盘上的 escape 键结合。当用户按下该键，消息框将执行cancel按钮事件
            0);                                                     ///---这里是 定义第三个按钮， 该例子 只是 了显示2个按钮
    }

    msgBox->show();
    if(msgBox->exec()==QMessageBox::Yes)
    {
        QSqlQuery query(db);
        query.prepare("DELETE FROM light_info WHERE light_number=? AND record_time=?");
        query.addBindValue(data.toString());
        query.addBindValue(data_time.toString());
        if(query.exec())
        {
            //刷新内容
            QMessageBox::information(this, tr("提示"),tr("删除成功！"));
            on_toolButton_clicked();
        }
        else
            QMessageBox::information(this, tr("提示"),tr("删除失败！"));
        query.finish();
    }
}
