#ifndef LIGHTINFOADDDIALOG_H
#define LIGHTINFOADDDIALOG_H

#include <QDialog>
#include <QString>
#include <QSqlDatabase>
#include <QtSql/QsqlQueryModel>
#include <QShowEvent>
namespace Ui {
class LightInfoAddDialog;
}

class LightInfoAddDialog : public QDialog
{
    Q_OBJECT
signals:
    void closeSign(int);
protected:
    void showEvent(QShowEvent *event);
public:
    explicit LightInfoAddDialog(QWidget *parent = 0);
    ~LightInfoAddDialog();

private slots:
    void on_pushButtonAdd_clicked();

    void on_pushButtonConcel_clicked();

    void setDatas(const QString str1,const QString str2,const QString str3,int s);
    void on_lineEditPlate_textChanged(const QString &arg1);

    void on_comboBox_currentIndexChanged(int index);

private:
    Ui::LightInfoAddDialog *ui;
    QString light_num;
    QString record_type;
    QString record_date;
    int sign;
};

#endif // LIGHTINFOADDDIALOG_H
