#ifndef TRAFFICVIOLATIONRECORD_H
#define TRAFFICVIOLATIONRECORD_H

#include <QWidget>
#include <QSqlDatabase>
#include <QtSql/QsqlQueryModel>
#include <QShowEvent>
namespace Ui {
class TrafficViolationRecord;
}

class TrafficViolationRecord : public QWidget
{
    Q_OBJECT

public:
    explicit TrafficViolationRecord(QWidget *parent = 0);
    ~TrafficViolationRecord();
protected:
    void showEvent(QShowEvent *event);
public:
    void init(int v);
private slots:
    void on_toolButton_clicked();

    void on_pushButtonQuery_clicked();

    void on_textEditCar_textChanged();

    void on_dateEditStart_dateChanged(const QDate &d);

    void on_dateEditStop_dateChanged(const QDate &d);

    void on_dateEditStart_timeChanged(const QTime &t);

    void on_dateEditStop_timeChanged(const QTime &t);

private:
    Ui::TrafficViolationRecord *ui;
    QSqlDatabase db;
    QSqlQueryModel *model1,*model2;
};

#endif // TRAFFICVIOLATIONRECORD_H
