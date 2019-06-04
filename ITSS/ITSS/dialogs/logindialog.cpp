#include "logindialog.h"
#include "ui_logindialog.h"
#include <QtSql>
#include <QSqlQuery>
#include <QSqlDatabase>
#include "../variabledef.h"
#include "../MyUtils/loginusrsingleton.h"
#include <QMessageBox>
LogInDialog::LogInDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LogInDialog)
{
    ui->setupUi(this);
    ui->toolButton->setIcon(QIcon("../images/Look.png"));
}

LogInDialog::~LogInDialog()
{
    delete ui;  
}

void LogInDialog::showEvent(QShowEvent *event)
{
    isPass = true;
    ui->pushButton->setEnabled(false);
    ui->lineEdit_2->setEchoMode(QLineEdit::PasswordEchoOnEdit);

    QRegExp user_reg_exp("[A-Za-z0-9_]{6,30}");
    QRegExpValidator *pReg = new QRegExpValidator(user_reg_exp);
    ui->lineEdit->setValidator(pReg);
    QRegExp password_reg_exp("[^\u4E00-\u9FA5]{6,20}");
    QRegExpValidator *password_validator = new QRegExpValidator(password_reg_exp);
    ui->lineEdit_2->setValidator(password_validator);
}

void LogInDialog::on_lineEdit_textChanged(const QString &arg1)
{
    if(arg1 != "" && ui->lineEdit_2->text() != "")
        ui->pushButton->setEnabled(true);
    else
        ui->pushButton->setEnabled(false);
}

void LogInDialog::on_lineEdit_2_textChanged(const QString &arg1)
{
    if(arg1 != "" && ui->lineEdit->text() != "")
        ui->pushButton->setEnabled(true);
    else
        ui->pushButton->setEnabled(false);
}

void LogInDialog::on_toolButton_clicked()
{
    ui->lineEdit_2->setEchoMode(isPass ? QLineEdit::Normal : QLineEdit::PasswordEchoOnEdit);
    isPass = !isPass;
}
//登录
void LogInDialog::on_pushButton_clicked()
{
    QSqlDatabase db;
    if(QSqlDatabase::contains("myConnection_login"))
        db = QSqlDatabase::database("myConnection_login");
    else
        db = QSqlDatabase::addDatabase("QSQLITE","myConnection_login");//添加数据库驱动
    db.setDatabaseName(DBPath);
    if(!db.open()) //打开数据库
    {
        QMessageBox::critical(0, qApp->tr("Cannot open database"),
                              qApp->tr("Unable to establish a database connection. "),
                              QMessageBox::Cancel);
        return;
    }
    QSqlQuery query(db);
    query.prepare("SELECT user_type,password FROM login_user WHERE name=?");
    query.addBindValue(ui->lineEdit->text());
    if(query.exec())
    {
        if(query.next())
        {
            int n1 = query.record().indexOf("user_type");
            int v1 = query.value(n1).toInt();
            int n2 = query.record().indexOf("password");
            QString v2 = query.value(n2).toString();
            if(v2 == ui->lineEdit_2->text())
            {
                LogInUsrSingleton *pSingleton1 = LogInUsrSingleton::getInstance();
                pSingleton1->setuserName(ui->lineEdit->text());
                pSingleton1->setauthority(v1);
                query.prepare("INSERT INTO login_record(name,user_type,login_time) VALUES(?,?,?)");
                query.addBindValue(ui->lineEdit->text());
                query.addBindValue(v1);
                QDateTime current_date_time = QDateTime::currentDateTime();
                QString cdt = current_date_time.toString("yyyy-MM-dd hh:mm:ss");
                query.addBindValue(cdt);
                query.exec();
                query.finish();
                if(db.open())
                    db.close();
                ui->lineEdit->clear();
                ui->lineEdit_2->clear();
                emit closeSign4(1);
                this->close();
            }else{
                ui->lineEdit_2->clear();
                ui->labelShow->setText("用户密码错误！");
            }
        }else{
            ui->lineEdit_2->clear();
            ui->labelShow->setText("该用户不存在！");
        }
    }else
    {
        ui->labelShow->setText("登录失败，请检查用户名输入是否合法");
    }
    query.finish();
    if(db.open())
        db.close();
    ui->pushButton->setEnabled(false);
}
//取消登录
void LogInDialog::on_pushButton_2_clicked()
{
    ui->lineEdit->clear();
    ui->lineEdit_2->clear();
    ui->labelShow->clear();
    emit closeSign4(0);
    this->close();
}
//注册
void LogInDialog::on_pushButton_3_clicked()
{
    emit closeSign4(2);
    this->close();
}
