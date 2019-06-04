#ifndef ETCRECHARGE_H
#define ETCRECHARGE_H

#include <QWidget>
#include <QSqlDatabase>
#include <QtSql/QsqlQueryModel>
#include <QShowEvent>
namespace Ui {
class EtcRecharge;
}

class EtcRecharge : public QWidget
{
    Q_OBJECT

public:
    explicit EtcRecharge(QWidget *parent = 0);
    ~EtcRecharge();
public:
    void init(int v);
private slots:
    void on_toolButton_clicked();

    void on_pushButtonRecharge_clicked();
    void on_textEditCar_cursorPositionChanged();

    void on_textEditMoney_cursorPositionChanged();

    void on_pushButtonQuery_clicked();

    void on_tableView_doubleClicked(const QModelIndex &index);

protected:
     void showEvent(QShowEvent *event);
private:
    Ui::EtcRecharge *ui;
    QSqlDatabase db;
    QSqlQueryModel *model;
};

#endif // ETCRECHARGE_H
