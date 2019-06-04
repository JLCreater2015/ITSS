#ifndef TRAFFICFLOW_H
#define TRAFFICFLOW_H

#include <QWidget>
#include <QShowEvent>
namespace Ui {
class TrafficFlow;
}

class TrafficFlow : public QWidget
{
    Q_OBJECT

public:
    explicit TrafficFlow(QWidget *parent = 0);
    ~TrafficFlow();
protected:
     void showEvent(QShowEvent *event);
private slots:

    void on_pushButtonDay_clicked();

    void on_pushButtonMonth_clicked();

    void on_pushButtonYear_clicked();

    void on_pushButtonFoward_clicked();

    void on_pushButtonLater_clicked();

    void on_toolButton_clicked();

private:
    void openButtons(bool b);

    void selectData(int t,int s);
private:
    Ui::TrafficFlow *ui;
    int sign;
};

#endif // TRAFFICFLOW_H
