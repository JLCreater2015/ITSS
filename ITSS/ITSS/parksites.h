#ifndef PARKSITES_H
#define PARKSITES_H

#include <QWidget>
#include <QSqlDatabase>
#include <QtSql/QsqlQueryModel>
#include <QShowEvent>
namespace Ui {
class ParkSites;
}

class ParkSites : public QWidget
{
    Q_OBJECT

public:
    explicit ParkSites(QWidget *parent = 0);
    ~ParkSites();
public:
    void init(int v);
private slots:
    void on_textEditCar_cursorPositionChanged();

    void on_textEditMoney_cursorPositionChanged();

    void on_pushButtonReset_clicked();

    void on_pushButtonRecharge_clicked();

    void on_pushButtonQuery_clicked();

    void on_toolButton_clicked();

    void on_tableView_doubleClicked(const QModelIndex &index);

protected:
    void showEvent(QShowEvent *event);
private:
    Ui::ParkSites *ui;
    QSqlDatabase db;
    QSqlQueryModel *model;
};

#endif // PARKSITES_H
