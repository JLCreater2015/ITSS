#ifndef ETCCARINFO_H
#define ETCCARINFO_H

#include <QWidget>
#include <QSqlDatabase>
#include <QtSql/QsqlQueryModel>
#include <QShowEvent>
#include "dialogs/etccarinfoadddialog.h"
#include "dialogs/etccarinfoupdatedialog.h"
namespace Ui {
class EtcCarInfo;
}

class EtcCarInfo : public QWidget
{
    Q_OBJECT

public:
    explicit EtcCarInfo(QWidget *parent = 0);
    ~EtcCarInfo();
signals:
    void carNumber(const QString);
public:
    void init(int v);
protected:
     void showEvent(QShowEvent *event);
private slots:
    void on_toolButton_clicked();
    void on_pushButtonAdd_clicked();

    void on_textEdit_cursorPositionChanged();

    void on_pushButtonDelete_clicked();

    void on_pushButtonUpdate_clicked();

    void on_pushButtonSelect_clicked();

    void on_textEdit_textChanged();

    void on_tableView_doubleClicked(const QModelIndex &index);

public slots:
    void sendOpenSignals(int v);

private:
    Ui::EtcCarInfo *ui;
    QSqlDatabase db;
    QSqlQueryModel *model;
    etcCarInfoAddDialog carInfoAddDialog;
    EtcCarInfoUpdateDialog etcCarInfoUpdateDialog;
};

#endif // ETCCARINFO_H
