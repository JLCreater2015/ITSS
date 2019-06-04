#include "parkuserinfo.h"
#include "ui_parkuserinfo.h"
#include <QtSql>
#include <QSqlQuery>
#include <QMessageBox>
#include "variabledef.h"
#include "MyUtils/loginusrsingleton.h"

ParkUserInfo::ParkUserInfo(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ParkUserInfo)
{
    ui->setupUi(this);
    //准备数据模型
    if(QSqlDatabase::contains("myConnection_parkinfo"))
        db = QSqlDatabase::database("myConnection_parkinfo");
    else
        db = QSqlDatabase::addDatabase("QSQLITE","myConnection_parkinfo");//添加数据库驱动
    db.setDatabaseName(DBPath);
    if(!db.open()) //打开数据库
    {
        QMessageBox::critical(0, qApp->tr("Cannot open database"),
                              qApp->tr("Unable to establish a database connection. "),
                              QMessageBox::Cancel);
        return;
    }
    model = new QSqlQueryModel(ui->tableView);
    QObject::connect(&parkUserInfoAddDialog,SIGNAL(closeSign(int)),this,SLOT(sendOpenSignals(int)));
    QObject::connect(&parkUserInfoUpdateDialog,SIGNAL(closeSign(int)),this,SLOT(sendOpenSignals(int)));
}

ParkUserInfo::~ParkUserInfo()
{
    if(db.open())
        db.close();
    if(model)
        delete model;
    delete ui;
}
void ParkUserInfo::init(int v)
{
    if(v == 0)
        model->setQuery(QString("SELECT userName,carNumber,cardID,carType,cardBalance,userPhone,registerTime FROM user_parking"),db);
    else
    {
        model->setQuery(QString("SELECT userName,carNumber,cardID,carType,cardBalance,userPhone,registerTime FROM user_parking WHERE carNumber='"
                                + ui->textEdit->toPlainText() + "'"),db);
    }
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("车主"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("车牌"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("标签号"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("车型"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("余额"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("车主电话"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("注册时间"));
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
    ui->tableView->setColumnWidth(0,85);
    ui->tableView->setColumnWidth(1,100);
    ui->tableView->setColumnWidth(2,130);
    ui->tableView->setColumnWidth(3,85);
    ui->tableView->setColumnWidth(4,75);
    ui->tableView->setColumnWidth(5,110);
    //坑：在进行表格设置时必须是“ui->tableView->setModel(model);”在前，属性具体设置在后，反之则设置不会生效。
    ui->tableView->show();
    ui->pushButtonSelect->setEnabled(false);
}

void ParkUserInfo::on_toolButton_clicked()
{
    model->clear();
    init(0);
}
void ParkUserInfo::showEvent(QShowEvent *event)
{
    ///关键点
    QObject::connect(this,SIGNAL(carNumber(const QString)),
                     &parkUserInfoUpdateDialog,SLOT(setDatas(const QString)));///坑：不能写成setDatas(const QString str)
    model->clear();
    init(0);
    LogInUsrSingleton *pSingleton1 = LogInUsrSingleton::getInstance();
    if(pSingleton1->getauthority() == 2)
    {
        ui->pushButtonAdd->setEnabled(false);
        ui->pushButtonDelete->setEnabled(false);
        ui->pushButtonUpdete->setEnabled(false);
    }
}

void ParkUserInfo::sendOpenSignals(int v)
{
    if(v == 1)
        QMessageBox::information(this, tr("提示"),tr("添加成功！"));
    else if(v == 2)
        QMessageBox::information(this, tr("提示"),tr("修改成功！"));
    else if(v == 3)
        QMessageBox::information(this, tr("提示"),tr("修改失败..."));
    else if(v == 4)
        QMessageBox::information(this, tr("提示"),tr("添加失败..."));
    else
        QMessageBox::information(this, tr("提示"),tr("取消操作。"));
    model->clear();
    init(0);
}

void ParkUserInfo::on_textEdit_textChanged()
{
    if(ui->textEdit->toPlainText() != "")
        ui->pushButtonSelect->setEnabled(true);
    else
        ui->pushButtonSelect->setEnabled(false);
}

void ParkUserInfo::on_textEdit_cursorPositionChanged()
{
    if(ui->textEdit->toPlainText() != "")
        ui->pushButtonSelect->setEnabled(true);
    else
        ui->pushButtonSelect->setEnabled(false);
}
//添加
void ParkUserInfo::on_pushButtonAdd_clicked()
{
    parkUserInfoAddDialog.show();
}
//删除
void ParkUserInfo::on_pushButtonDelete_clicked()
{
    on_tableView_doubleClicked(ui->tableView->currentIndex());
}
//修改
void ParkUserInfo::on_pushButtonUpdete_clicked()
{
    int row= ui->tableView->currentIndex().row();
    if(row < 0)
        return;
    QAbstractItemModel *mod = ui->tableView->model();
    QModelIndex index = mod->index(row,1);
    QVariant data = mod->data(index);
    emit carNumber(data.toString());
    parkUserInfoUpdateDialog.show();
}
//双击一行删除
void ParkUserInfo::on_tableView_doubleClicked(const QModelIndex &index)
{   
    LogInUsrSingleton *pSingleton1 = LogInUsrSingleton::getInstance();
    if(pSingleton1->getauthority() == 2)
        return;

    int row= index.row();
    if(row < 0)
        return;
    QAbstractItemModel *mod = ui->tableView->model();
    QModelIndex index0 = mod->index(row,1);//选中行第二列的内容
    QVariant data = mod->data(index0);
    ///---实例化消息对话框对象
    QMessageBox *msgBox;
    msgBox = new QMessageBox(tr("Confirm"),       ///--这里是设置消息框标题
        "确认删除车牌号为 " + data.toString() + " 的用户信息?",                       ///--这里是设置消息框显示的内容
        QMessageBox::Warning,                          ///--这里是在消息框显示的图标
        QMessageBox::Yes | QMessageBox::Default,     ///---这里是显示消息框上的按钮情况
        QMessageBox::Cancel | QMessageBox::Escape,  ///---这里与 键盘上的 escape 键结合。当用户按下该键，消息框将执行cancel按钮事件
        0);                                                     ///---这里是 定义第三个按钮， 该例子 只是 了显示2个按钮

    msgBox->show();
    if(msgBox->exec() == QMessageBox::Yes)
    {
        QSqlQuery query(db);
        query.prepare("DELETE FROM user_parking WHERE carNumber=?");
        query.addBindValue(data.toString());
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
//查询
void ParkUserInfo::on_pushButtonSelect_clicked()
{
    model->clear();
    init(1);
    ui->pushButtonSelect->setEnabled(false);
}
