#include "dialogs/parkuserinfoadddialog.h"
#include "ui_parkuserinfoadddialog.h"
#include "../variabledef.h"
#include <QtSql>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QMessageBox>
ParkUserInfoAddDialog::ParkUserInfoAddDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ParkUserInfoAddDialog)
{
    ui->setupUi(this);
    plate_edited = false;
    name_edited = false;
    tag_edited = false;
    contact_edited = false;
    ui->pushButtonAdd->setEnabled(false);
    ui->lineEditCharge->setText("0");
}

ParkUserInfoAddDialog::~ParkUserInfoAddDialog()
{
    delete ui;
}

void ParkUserInfoAddDialog::on_lineEditPlate_textChanged(const QString &arg1)
{
    if(arg1 != "")
    {
        plate_edited = true;
        if(name_edited && tag_edited && contact_edited)
            ui->pushButtonAdd->setEnabled(true);
    }
    else
        plate_edited = false;
}

void ParkUserInfoAddDialog::on_lineEditName_textChanged(const QString &arg1)
{
    if(arg1 != "")
    {
        name_edited = true;
        if(plate_edited && tag_edited && contact_edited)
            ui->pushButtonAdd->setEnabled(true);
    }
    else
        name_edited = false;
}

void ParkUserInfoAddDialog::on_lineEditTag_textChanged(const QString &arg1)
{
    if(arg1 != "")
    {
        tag_edited = true;
        if(plate_edited && name_edited && contact_edited)
            ui->pushButtonAdd->setEnabled(true);
    }
    else
        tag_edited = false;
}

void ParkUserInfoAddDialog::on_lineEditContact_textChanged(const QString &arg1)
{
    if(arg1 != "")
    {
        contact_edited = true;
        if(plate_edited && name_edited && tag_edited)
            ui->pushButtonAdd->setEnabled(true);
    }
    else
        contact_edited = false;
}

void ParkUserInfoAddDialog::on_pushButtonAdd_clicked()
{
    QSqlDatabase db;
    ///数据库连接命名不能起冲突
    if(QSqlDatabase::contains("myConnection_parkadd"))
        db = QSqlDatabase::database("myConnection_parkadd");
    else
        db = QSqlDatabase::addDatabase("QSQLITE","myConnection_parkadd");//添加数据库驱动
    db.setDatabaseName(DBPath);
    if(!db.open())
    {
        QMessageBox::critical(0, qApp->tr("Cannot open database"),
                              qApp->tr("Unable to establish a database connection. "),
                              QMessageBox::Cancel);
        return;
    }
    QSqlQuery query(db);
    query.prepare("INSERT INTO user_parking(userName,carNumber,cardID,carType,cardBalance,userPhone,registerTime) VALUES(?,?,?,?,?,?,?)");
    query.addBindValue(ui->lineEditName->text());
    query.addBindValue(ui->lineEditPlate->text());
    query.addBindValue(ui->lineEditTag->text());
    query.addBindValue(ui->comboBox->currentText());
    query.addBindValue(ui->lineEditCharge->text());
    query.addBindValue(ui->lineEditContact->text());
    query.addBindValue(QDate::currentDate().toString("yyyy-MM-dd"));
    if(query.exec())
    {
        emit closeSign(1);
    }
    else
    {
        emit closeSign(4);
    }
    query.finish();
    db.close();
    this->close();
}

void ParkUserInfoAddDialog::on_pushButtonConcel_clicked()
{
    emit closeSign(0);
    this->close();
}

void ParkUserInfoAddDialog::on_lineEditCharge_editingFinished()
{
    if(ui->lineEditCharge->text() == "")
        ui->lineEditCharge->setText("0");
}
