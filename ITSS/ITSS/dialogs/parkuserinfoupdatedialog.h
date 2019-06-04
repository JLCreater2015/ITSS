#ifndef PARKUSERINFOUPDATEDIALOG_H
#define PARKUSERINFOUPDATEDIALOG_H

#include <QDialog>

namespace Ui {
class ParkUserInfoUpdateDialog;
}

class ParkUserInfoUpdateDialog : public QDialog
{
    Q_OBJECT
signals:
    void closeSign(int);
private slots:
    void setDatas(const QString str);
    void on_pushButtonUpdate_clicked();

    void on_pushButtonConcel_clicked();

    void on_lineEditName_textChanged(const QString &arg1);

    void on_lineEditTag_textChanged(const QString &arg1);

    void on_comboBox_currentIndexChanged(const QString &arg1);

    void on_lineEditContact_textChanged(const QString &arg1);

    void on_lineEditCharge_textChanged(const QString &arg1);

public:
    explicit ParkUserInfoUpdateDialog(QWidget *parent = 0);
    ~ParkUserInfoUpdateDialog();

private:
    Ui::ParkUserInfoUpdateDialog *ui;
};

#endif // PARKUSERINFOUPDATEDIALOG_H
