#include "dialogs/parkuserinfoupdatedialog.h"
#include "ui_parkuserinfoupdatedialog.h"
#include "../variabledef.h"
#include <QtSql>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QMessageBox>
ParkUserInfoUpdateDialog::ParkUserInfoUpdateDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ParkUserInfoUpdateDialog)
{
    ui->setupUi(this);
    ui->pushButtonUpdate->setEnabled(false);
}

ParkUserInfoUpdateDialog::~ParkUserInfoUpdateDialog()
{
    delete ui;
}

void ParkUserInfoUpdateDialog::setDatas(const QString str)
{
    QSqlDatabase db;
    if(QSqlDatabase::contains("myConnection_parkupdate"))
        db = QSqlDatabase::database("myConnection_parkupdate");
    else
        db = QSqlDatabase::addDatabase("QSQLITE","myConnection_parkupdate");//添加数据库驱动
    db.setDatabaseName(DBPath);
    if(!db.open())
    {
        QMessageBox::critical(0, qApp->tr("Cannot open database"),
                              qApp->tr("Unable to establish a database connection. "),
                              QMessageBox::Cancel);
        return;
    }
    QSqlQuery query(db);
    query.prepare("SELECT userName,cardID,carType,cardBalance,userPhone FROM user_parking WHERE carNumber=?");
    query.addBindValue(str);
    if(query.exec())
    {
        if(query.next() && query.record().count() >= 5)
        {
            ui->lineEditName->setText(query.value(0).toString());
            ui->lineEditPlate->setText(str);
            ui->lineEditTag->setText(query.value(1).toString());
            ui->lineEditContact->setText(query.value(4).toString());
            ui->lineEditCharge->setText(query.value(3).toString());
            if(query.value(2).toString() == "大型")
                ui->comboBox->setCurrentIndex(0);
            else if(query.value(2).toString() == "中型")
                ui->comboBox->setCurrentIndex(1);
            else if(query.value(2).toString() == "小型")
                ui->comboBox->setCurrentIndex(2);
            else if(query.value(2).toString() == "公交车")
                ui->comboBox->setCurrentIndex(3);
            else if(query.value(2).toString() == "警车")
                ui->comboBox->setCurrentIndex(4);
        }
    }
    else
    {
        QMessageBox::information(this, tr("提示"),tr("没有相关信息！"));
    }
    query.finish();
    db.close();
}

void ParkUserInfoUpdateDialog::on_pushButtonUpdate_clicked()
{
    QSqlDatabase db;
    if(QSqlDatabase::contains("myConnection_update"))
        db = QSqlDatabase::database("myConnection_update");
    else
        db = QSqlDatabase::addDatabase("QSQLITE","myConnection_update");//添加数据库驱动
    db.setDatabaseName(DBPath);
    if(!db.open())
    {
        QMessageBox::critical(0, qApp->tr("Cannot open database"),
                              qApp->tr("Unable to establish a database connection. "),
                              QMessageBox::Cancel);
        return;
    }
    QSqlQuery query(db);
    query.prepare("UPDATE user_parking SET userName=?,cardID=?,carType=?,cardBalance=?,userPhone=? WHERE carNumber=?");
    query.addBindValue(ui->lineEditName->text());
    query.addBindValue(ui->lineEditTag->text());
    query.addBindValue(ui->comboBox->currentText());
    query.addBindValue(ui->lineEditCharge->text());
    query.addBindValue(ui->lineEditContact->text());
    query.addBindValue(ui->lineEditPlate->text());
    if(query.exec())
    {
        emit closeSign(2);
    }
    else
        emit closeSign(3);
    query.finish();
    db.close();
    this->close();
}

void ParkUserInfoUpdateDialog::on_pushButtonConcel_clicked()
{
    emit closeSign(0);
    this->close();
}

void ParkUserInfoUpdateDialog::on_lineEditName_textChanged(const QString &arg1)
{
    if(arg1 != "")
        ui->pushButtonUpdate->setEnabled(true);
    else
        ui->pushButtonUpdate->setEnabled(false);
}

void ParkUserInfoUpdateDialog::on_lineEditTag_textChanged(const QString &arg1)
{
    if(arg1 != "")
        ui->pushButtonUpdate->setEnabled(true);
    else
        ui->pushButtonUpdate->setEnabled(false);
}

void ParkUserInfoUpdateDialog::on_comboBox_currentIndexChanged(const QString &arg1)
{
    ui->pushButtonUpdate->setEnabled(true);
}

void ParkUserInfoUpdateDialog::on_lineEditContact_textChanged(const QString &arg1)
{
    if(arg1 != "")
        ui->pushButtonUpdate->setEnabled(true);
    else
        ui->pushButtonUpdate->setEnabled(false);
}

void ParkUserInfoUpdateDialog::on_lineEditCharge_textChanged(const QString &arg1)
{
    if(arg1 != "")
        ui->pushButtonUpdate->setEnabled(true);
    else
        ui->pushButtonUpdate->setEnabled(false);
}
