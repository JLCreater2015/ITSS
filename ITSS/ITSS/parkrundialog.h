#ifndef PARKRUNDIALOG_H
#define PARKRUNDIALOG_H

#include <QDialog>
#include <QCloseEvent>
#include <QShowEvent>
#include <QStringList>
#include <QtSql>
#include <QSqlDatabase>
#include <QSqlQuery>

namespace Ui {
class ParkRunDialog;
}

class ParkRunDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ParkRunDialog(QWidget *parent = 0);
    ~ParkRunDialog();
signals:
    void closeSign2(int);

protected:
    void closeEvent(QCloseEvent *event);
    void showEvent(QShowEvent *event);

private slots:
    void on_pushButtonEmpty_clicked();
    void showInfoIntime(QString m);
public:
    void init();

private:
    Ui::ParkRunDialog *ui;
    int rowCount;
    QSqlDatabase db;
};

#endif // PARKRUNDIALOG_H
