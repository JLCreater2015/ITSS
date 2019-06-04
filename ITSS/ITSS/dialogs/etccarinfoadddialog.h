#ifndef ETCCARINFOADDDIALOG_H
#define ETCCARINFOADDDIALOG_H

#include <QDialog>

namespace Ui {
class etcCarInfoAddDialog;
}

class etcCarInfoAddDialog : public QDialog
{
    Q_OBJECT

public:
    explicit etcCarInfoAddDialog(QWidget *parent = 0);
    ~etcCarInfoAddDialog();
signals:
    void closeSign(int);
private slots:
    void on_pushButtonAdd_clicked();

    void on_pushButtonConcel_clicked();

    void on_lineEditCharge_editingFinished();

    void on_lineEditPlate_textChanged(const QString &arg1);

    void on_lineEditName_textChanged(const QString &arg1);

    void on_lineEditTag_textChanged(const QString &arg1);

    void on_lineEditContact_textChanged(const QString &arg1);

private:
    Ui::etcCarInfoAddDialog *ui;
    bool plate_edited;
    bool name_edited;
    bool tag_edited;
    bool contact_edited;
};

#endif // ETCCARINFOADDDIALOG_H
