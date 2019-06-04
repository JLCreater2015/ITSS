#ifndef PARKUSERINFOADDDIALOG_H
#define PARKUSERINFOADDDIALOG_H

#include <QDialog>

namespace Ui {
class ParkUserInfoAddDialog;
}

class ParkUserInfoAddDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ParkUserInfoAddDialog(QWidget *parent = 0);
    ~ParkUserInfoAddDialog();

signals:
    void closeSign(int);
private slots:
    void on_lineEditPlate_textChanged(const QString &arg1);

    void on_lineEditName_textChanged(const QString &arg1);

    void on_lineEditTag_textChanged(const QString &arg1);

    void on_lineEditContact_textChanged(const QString &arg1);

    void on_pushButtonAdd_clicked();

    void on_pushButtonConcel_clicked();

    void on_lineEditCharge_editingFinished();

private:
    Ui::ParkUserInfoAddDialog *ui;
    bool plate_edited;
    bool name_edited;
    bool tag_edited;
    bool contact_edited;
};

#endif // PARKUSERINFOADDDIALOG_H
