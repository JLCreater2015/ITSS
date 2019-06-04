#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include <QShowEvent>
namespace Ui {
class LogInDialog;
}

class LogInDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LogInDialog(QWidget *parent = 0);
    ~LogInDialog();
protected:
     void showEvent(QShowEvent *event);
private slots:
    void on_lineEdit_textChanged(const QString &arg1);

    void on_lineEdit_2_textChanged(const QString &arg1);

    void on_toolButton_clicked();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

signals:
    void closeSign4(int);

private:
    Ui::LogInDialog *ui;
    bool isPass;
};

#endif // LOGINDIALOG_H
