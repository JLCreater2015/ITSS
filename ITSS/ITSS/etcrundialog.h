#ifndef ETCRUNDIALOG_H
#define ETCRUNDIALOG_H

#include <QDialog>
#include <QCloseEvent>
#include <QStringList>
#include <QtSql>
#include <QSqlDatabase>
#include <QShowEvent>
#include <QSqlQuery>
namespace Ui {
class ETCRunDialog;
}

class ETCRunDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ETCRunDialog(QWidget *parent = 0);
    ~ETCRunDialog();

private slots:
    void showInfoIntime(QString m);

    void on_pushButtonEmpty_clicked();

public:
    void init();

signals:
    void closeSign1(int);

protected:
     void closeEvent(QCloseEvent *event);
     void showEvent(QShowEvent *event);

private:
    Ui::ETCRunDialog *ui;
    int rowCount;
    QSqlDatabase db;
};

#endif // ETCRUNDIALOG_H
