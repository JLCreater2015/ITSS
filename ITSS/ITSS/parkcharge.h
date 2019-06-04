#ifndef PARKCHARGE_H
#define PARKCHARGE_H

#include <QWidget>
#include <QSqlDatabase>
#include <QtSql/QsqlQueryModel>
#include <QShowEvent>
namespace Ui {
class ParkCharge;
}

class ParkCharge : public QWidget
{
    Q_OBJECT

public:
    explicit ParkCharge(QWidget *parent = 0);
    ~ParkCharge();
public:
    void init(int v);
private slots:
    void on_toolButton_clicked();
    void on_textEditCar_cursorPositionChanged();

    void on_dateEditStop_userDateChanged(const QDate &d);

    void on_dateEditStart_userDateChanged(const QDate &d);

    void on_pushButtonQuery_clicked();

    void on_dateEditStart_dateChanged(const QDate &d);

    void on_dateEditStart_timeChanged(const QTime &t);

    void on_dateEditStop_dateChanged(const QDate &d);

    void on_dateEditStop_timeChanged(const QTime &t);

protected:
    void showEvent(QShowEvent *event);
private:
    Ui::ParkCharge *ui;
    QSqlDatabase db;
    QSqlQueryModel *model;
};

#endif // PARKCHARGE_H
