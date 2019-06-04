#include "lightinfoadddialog.h"
#include "ui_lightinfoadddialog.h"
#include "../variabledef.h"
#include <QtSql>
#include <QSqlQuery>
#include <QMessageBox>
LightInfoAddDialog::LightInfoAddDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LightInfoAddDialog)
{
    ui->setupUi(this);
}

LightInfoAddDialog::~LightInfoAddDialog()
{
    delete ui;
}

void LightInfoAddDialog::on_pushButtonAdd_clicked()
{
    QSqlDatabase db;
    ///数据库连接命名不能起冲突
    if(QSqlDatabase::contains("myConnection_lightadd"))
        db = QSqlDatabase::database("myConnection_lightadd");
    else
        db = QSqlDatabase::addDatabase("QSQLITE","myConnection_lightadd");//添加数据库驱动
    db.setDatabaseName(DBPath);
    if(!db.open())
    {
        QMessageBox::critical(0, qApp->tr("Cannot open database"),
                              qApp->tr("Unable to establish a database connection. "),
                              QMessageBox::Cancel);
        return;
    }
    QSqlQuery query(db);

    if(light_num == "")
    {
        query.prepare("INSERT INTO light_info(light_number,record_type,type,record_time) VALUES(?,?,?,?)");
        query.addBindValue(ui->lineEditPlate->text());
        query.addBindValue(ui->comboBox->currentText());
        query.addBindValue(QString::number(sign, 10));
        query.addBindValue(ui->dateTimeEdit->text());
    }else{
        query.prepare("UPDATE light_info SET record_type=? WHERE light_number=? AND record_time=?");
        query.addBindValue(ui->comboBox->currentText());
        query.addBindValue(ui->lineEditPlate->text());
        query.addBindValue(ui->dateTimeEdit->text());
    }
    if(query.exec())
    {
        emit closeSign(1);
    }
    else
    {
        emit closeSign(2);
    }
    query.finish();
    db.close();

    light_num = "";
    record_type = "";
    record_date = "";
    sign = 0;
    ui->lineEditPlate->setText("");
    this->close();
}

void LightInfoAddDialog::on_pushButtonConcel_clicked()
{
    emit closeSign(0);

    light_num = "";
    record_type = "";
    record_date = "";
    sign = 0;
    ui->lineEditPlate->setText("");
    this->close();
}

void LightInfoAddDialog::showEvent(QShowEvent *event)
{
    ui->lineEditPlate->setEnabled(true);
    ui->dateTimeEdit->setDisplayFormat("yyyy-MM-dd HH:mm:ss");
    if(light_num != "")
    {
        ui->lineEditPlate->setText(light_num);
        if(record_type == "投入使用")
            ui->comboBox->setCurrentIndex(0);
        else if(record_type == "故障")
            ui->comboBox->setCurrentIndex(1);
        else if(record_type == "恢复")
            ui->comboBox->setCurrentIndex(2);
        else if(record_type == "更换")
            ui->comboBox->setCurrentIndex(3);
        else if(record_type == "撤除")
            ui->comboBox->setCurrentIndex(4);
        else
            ui->comboBox->setCurrentIndex(0);

        ui->dateTimeEdit->setDateTime(QDateTime::fromString(record_date,"yyyy-MM-dd HH:mm:ss"));
        ui->lineEditPlate->setEnabled(false);
        ui->dateTimeEdit->setEnabled(false);
    }
    ui->pushButtonAdd->setEnabled(false);
}

void LightInfoAddDialog::setDatas(const QString str1,const QString str2,const QString str3,int s)
{
    light_num = str1;
    record_type = str2;
    record_date = str3;
    sign = s;
}

void LightInfoAddDialog::on_lineEditPlate_textChanged(const QString &arg1)
{
    if(arg1 != "")
        ui->pushButtonAdd->setEnabled(true);
    else
        ui->pushButtonAdd->setEnabled(false);
}

void LightInfoAddDialog::on_comboBox_currentIndexChanged(int index)
{
    if(light_num != "")
    {
        ui->pushButtonAdd->setEnabled(true);
    }
}
