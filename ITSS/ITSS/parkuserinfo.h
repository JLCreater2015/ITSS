#ifndef PARKUSERINFO_H
#define PARKUSERINFO_H

#include <QWidget>
#include <QSqlDatabase>
#include <QtSql/QsqlQueryModel>
#include <QShowEvent>
#include "dialogs/parkuserinfoadddialog.h"
#include "dialogs/parkuserinfoupdatedialog.h"
namespace Ui {
class ParkUserInfo;
}

class ParkUserInfo : public QWidget
{
    Q_OBJECT

public:
    explicit ParkUserInfo(QWidget *parent = 0);
    ~ParkUserInfo();
signals:
    void carNumber(const QString);
public:
    void init(int v);
private slots:
    void on_toolButton_clicked();

    void on_textEdit_textChanged();

    void on_textEdit_cursorPositionChanged();

    void on_pushButtonAdd_clicked();

    void on_pushButtonDelete_clicked();

    void on_pushButtonUpdete_clicked();

    void on_tableView_doubleClicked(const QModelIndex &index);

    void on_pushButtonSelect_clicked();
public slots:
    void sendOpenSignals(int v);
protected:
    void showEvent(QShowEvent *event);
private:
    Ui::ParkUserInfo *ui;
    QSqlDatabase db;
    QSqlQueryModel *model;
    ParkUserInfoAddDialog parkUserInfoAddDialog;
    ParkUserInfoUpdateDialog parkUserInfoUpdateDialog;
};

#endif // PARKUSERINFO_H
