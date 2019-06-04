#ifndef TRAFFICVIOLATIONDIALOG_H
#define TRAFFICVIOLATIONDIALOG_H

#include <QDialog>
#include <QCloseEvent>
#include <QStringList>
#include <QtSql>
#include <QSqlDatabase>
#include <QShowEvent>
#include <QSqlQuery>
namespace Ui {
class TrafficViolationDialog;
}

class TrafficViolationDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TrafficViolationDialog(QWidget *parent = 0);
    ~TrafficViolationDialog();

private slots:
    void on_pushButtonEmpty_clicked();

    void showInfoIntime(QString m);
public:
    void init();

signals:
    void closeSign3(int);

protected:
     void closeEvent(QCloseEvent *event);
private:
    Ui::TrafficViolationDialog *ui;
    int rowCount;
    QSqlDatabase db;
};

#endif // TRAFFICVIOLATIONDIALOG_H
