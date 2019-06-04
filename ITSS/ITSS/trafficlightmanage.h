#ifndef TRAFFICLIGHTMANAGE_H
#define TRAFFICLIGHTMANAGE_H

#include <QWidget>
#include <QSqlDatabase>
#include <QtSql/QsqlQueryModel>
#include <QShowEvent>
#include "dialogs/lightinfoadddialog.h"
namespace Ui {
class TrafficLightManage;
}

class TrafficLightManage : public QWidget
{
    Q_OBJECT

public:
    explicit TrafficLightManage(QWidget *parent = 0);
    ~TrafficLightManage();

signals:
    void carNumber(const QString,const QString,const QString,int);
protected slots:
    void setLightType(int v);
private slots:
    void on_pushButtonAdd_clicked();
    void on_pushButtonDelete_clicked();

    void on_pushButtonUpdate_clicked();

    void on_pushButtonSelect_clicked();

    void on_toolButton_clicked();

    void on_textEdit_textChanged();

    void on_tableView_doubleClicked(const QModelIndex &index);
public slots:
    void sendOpenSignals(int v);
protected:
    void init(int v);
    void showEvent(QShowEvent *event);
private:
    Ui::TrafficLightManage *ui;
    int sign;
    QSqlDatabase db;
    QSqlQueryModel *model;
    LightInfoAddDialog lightInfoAddDialog;
};

#endif // TRAFFICLIGHTMANAGE_H
