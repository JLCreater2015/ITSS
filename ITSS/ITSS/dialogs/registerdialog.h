#ifndef REGISTERDIALOG_H
#define REGISTERDIALOG_H

#include <QDialog>
#include <QShowEvent>
namespace Ui {
class RegisterDialog;
}

class RegisterDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RegisterDialog(QWidget *parent = 0);
    ~RegisterDialog();
protected:
     void showEvent(QShowEvent *event);
private slots:
    void on_pushButtonOK_clicked();

    void on_pushButtonConcel_clicked();

    void on_lineEditName_textChanged(const QString &arg1);

    void on_lineEditPass_textChanged(const QString &arg1);

    void on_lineEditPassword_textChanged(const QString &arg1);

    void on_lineEditTel_textChanged(const QString &arg1);
signals:
    void closeSign5(int);
private:
    Ui::RegisterDialog *ui;
};

#endif // REGISTERDIALOG_H
