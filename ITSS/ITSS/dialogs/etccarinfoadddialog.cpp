#include "dialogs/etccarinfoadddialog.h"
#include "ui_etccarinfoadddialog.h"
#include "../variabledef.h"
#include <QtSql>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QMessageBox>

etcCarInfoAddDialog::etcCarInfoAddDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::etcCarInfoAddDialog)
{
    ui->setupUi(this);
    plate_edited = false;
    name_edited = false;
    tag_edited = false;
    contact_edited = false;
    ui->pushButtonAdd->setEnabled(false);
    ui->lineEditCharge->setText("0");
}

etcCarInfoAddDialog::~etcCarInfoAddDialog()
{
    delete ui;
}

void etcCarInfoAddDialog::on_pushButtonAdd_clicked()
{
    QSqlDatabase db;
    ///数据库连接命名不能起冲突
    if(QSqlDatabase::contains("myConnection_add"))
        db = QSqlDatabase::database("myConnection_add");
    else
        db = QSqlDatabase::addDatabase("QSQLITE","myConnection_add");//添加数据库驱动
    db.setDatabaseName(DBPath);
    if(!db.open())
    {
        QMessageBox::critical(0, qApp->tr("Cannot open database"),
                              qApp->tr("Unable to establish a database connection. "),
                              QMessageBox::Cancel);
        return;
    }
    QSqlQuery query(db);
    query.prepare("INSERT INTO users(name,carNum,tagID,carType,balance,tel) VALUES(?,?,?,?,?,?)");
    query.addBindValue(ui->lineEditName->text());
    query.addBindValue(ui->lineEditPlate->text());
    query.addBindValue(ui->lineEditTag->text());
    query.addBindValue(ui->comboBox->currentText());
    query.addBindValue(ui->lineEditCharge->text());
    query.addBindValue(ui->lineEditContact->text());
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

void etcCarInfoAddDialog::on_pushButtonConcel_clicked()
{
    emit closeSign(0);
    this->close(); 
}

void etcCarInfoAddDialog::on_lineEditCharge_editingFinished()
{
    if(ui->lineEditCharge->text() == "")
        ui->lineEditCharge->setText("0");
}

void etcCarInfoAddDialog::on_lineEditPlate_textChanged(const QString &arg1)
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

void etcCarInfoAddDialog::on_lineEditName_textChanged(const QString &arg1)
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

void etcCarInfoAddDialog::on_lineEditTag_textChanged(const QString &arg1)
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

void etcCarInfoAddDialog::on_lineEditContact_textChanged(const QString &arg1)
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
