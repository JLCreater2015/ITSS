#include "registerdialog.h"
#include "ui_registerdialog.h"
#include <QtSql>
#include <QSqlQuery>
#include <QSqlDatabase>
#include "../variabledef.h"
#include <QMessageBox>
RegisterDialog::RegisterDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RegisterDialog)
{
    ui->setupUi(this);
}

void RegisterDialog::showEvent(QShowEvent *event)
{
    ui->lineEditPass->setEchoMode(QLineEdit::PasswordEchoOnEdit);
    ui->lineEditPassword->setEchoMode(QLineEdit::PasswordEchoOnEdit);
    ui->pushButtonOK->setEnabled(false);
    QRegExp user_reg_exp("[A-Za-z0-9_]{6,30}");
    QRegExpValidator *pReg = new QRegExpValidator(user_reg_exp);
    ui->lineEditName->setValidator(pReg);
    QRegExp password_reg_exp("[^\u4E00-\u9FA5]{6,20}");
    QRegExpValidator *password_validator = new QRegExpValidator(password_reg_exp);
    ui->lineEditPass->setValidator(password_validator);
    ui->lineEditPassword->setValidator(password_validator);
}

RegisterDialog::~RegisterDialog()
{
    delete ui;
}

void RegisterDialog::on_pushButtonOK_clicked()
{
    QSqlDatabase db;
    if(QSqlDatabase::contains("myConnection_register"))
        db = QSqlDatabase::database("myConnection_register");
    else
        db = QSqlDatabase::addDatabase("QSQLITE","myConnection_register");//添加数据库驱动
    db.setDatabaseName(DBPath);
    if(!db.open()) //打开数据库
    {
        QMessageBox::critical(0, qApp->tr("Cannot open database"),
                              qApp->tr("Unable to establish a database connection. "),
                              QMessageBox::Cancel);
        return;
    }
    QSqlQuery query(db);
    query.prepare("INSERT INTO login_user(name,user_type,password,tel,register_time) VALUES(?,?,?,?,?)");
    query.addBindValue(ui->lineEditName->text());
    query.addBindValue(ui->comboBox->currentIndex() + 1);
    query.addBindValue(ui->lineEditPass->text());
    query.addBindValue(ui->lineEditTel->text());
    QDateTime current_date_time = QDateTime::currentDateTime();
    QString cdt = current_date_time.toString("yyyy-MM-dd hh:mm:ss");
    query.addBindValue(cdt);
    if(query.exec())
    {
        emit closeSign5(1);
        ui->lineEditName->clear();
        ui->lineEditPass->clear();
        ui->lineEditPassword->clear();
        ui->lineEditTel->clear();
        ui->labelShow->clear();
        this->close();
    }
    else
    {
        ui->lineEditName->clear();
        ui->lineEditPass->clear();
        ui->lineEditPassword->clear();
        ui->lineEditTel->clear();
        ui->labelShow->clear();
        ui->pushButtonOK->setEnabled(false);
        ui->labelShow->setText("注册失败!用户名已存在...");
    }
    query.finish();
    if(db.open())
        db.close();
}

void RegisterDialog::on_pushButtonConcel_clicked()
{
    emit closeSign5(0);
    ui->lineEditName->clear();
    ui->lineEditPass->clear();
    ui->lineEditPassword->clear();
    ui->lineEditTel->clear();
    ui->labelShow->clear();
    this->close();
}

void RegisterDialog::on_lineEditName_textChanged(const QString &arg1)
{
    if(arg1 != "" && ui->lineEditPass->text() != "" && ui->lineEditPass->text() == ui->lineEditPassword->text() && ui->lineEditTel->text() != "")
        ui->pushButtonOK->setEnabled(true);
    else
        ui->pushButtonOK->setEnabled(false);
    if(arg1 == "admin")
    {
        ui->pushButtonOK->setEnabled(false);
        ui->labelShow->setText("用户名不能为 admin !");
    }
}

void RegisterDialog::on_lineEditPass_textChanged(const QString &arg1)
{
    if(ui->lineEditName->text() != "" && arg1 != "" && arg1 == ui->lineEditPassword->text() && ui->lineEditTel->text() != "")
        ui->pushButtonOK->setEnabled(true);
    else
        ui->pushButtonOK->setEnabled(false);
    if(ui->lineEditName->text() == "admin")
    {
        ui->pushButtonOK->setEnabled(false);
        ui->labelShow->setText("用户名不能为 admin !");
    }
}

void RegisterDialog::on_lineEditPassword_textChanged(const QString &arg1)
{
    if(ui->lineEditName->text() != "" && arg1 != "" && ui->lineEditPass->text() == arg1 && ui->lineEditTel->text() != "")
        ui->pushButtonOK->setEnabled(true);
    else
        ui->pushButtonOK->setEnabled(false);
    if(ui->lineEditName->text() == "admin")
    {
        ui->pushButtonOK->setEnabled(false);
        ui->labelShow->setText("用户名不能为 admin !");
    }
}

void RegisterDialog::on_lineEditTel_textChanged(const QString &arg1)
{
    if(arg1 != "" && ui->lineEditPass->text() != "" && ui->lineEditPass->text() == ui->lineEditPassword->text() && ui->lineEditName->text() != "")
        ui->pushButtonOK->setEnabled(true);
    else
        ui->pushButtonOK->setEnabled(false);
    if(ui->lineEditName->text() == "admin")
    {
        ui->pushButtonOK->setEnabled(false);
        ui->labelShow->setText("用户名不能为 admin !");
    }
}
