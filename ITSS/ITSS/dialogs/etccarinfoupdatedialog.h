#ifndef ETCCARINFOUPDATEDIALOG_H
#define ETCCARINFOUPDATEDIALOG_H

#include <QDialog>

namespace Ui {
class EtcCarInfoUpdateDialog;
}

class EtcCarInfoUpdateDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EtcCarInfoUpdateDialog(QWidget *parent = 0);
    ~EtcCarInfoUpdateDialog();
signals:
    void closeSign(int);
private slots:
    void setDatas(const QString str);

    void on_comboBox_currentTextChanged(const QString &arg1);

    void on_pushButtonUpdate_clicked();

    void on_pushButtonConcel_clicked();

    void on_lineEditName_textChanged(const QString &arg1);

    void on_lineEditTag_textChanged(const QString &arg1);

    void on_lineEditContact_textChanged(const QString &arg1);

    void on_lineEditCharge_textChanged(const QString &arg1);

private:
    Ui::EtcCarInfoUpdateDialog *ui;
};

#endif // ETCCARINFOUPDATEDIALOG_H
