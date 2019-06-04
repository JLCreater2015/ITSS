#include "trafficflow.h"
#include "ui_trafficflow.h"
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QGridLayout>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QLegend>
#include <QtCharts/QBarCategoryAxis>
using namespace QtCharts;
TrafficFlow::TrafficFlow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TrafficFlow)
{
    ui->setupUi(this);
    sign = 0;
}

TrafficFlow::~TrafficFlow()
{
    delete ui;
}

void TrafficFlow::showEvent(QShowEvent *event)
{
    on_pushButtonDay_clicked();
}

void TrafficFlow::openButtons(bool b)
{
    ui->pushButtonDay->setEnabled(b);
    ui->pushButtonMonth->setEnabled(b);
    ui->pushButtonYear->setEnabled(b);
}

void TrafficFlow::on_pushButtonDay_clicked()
{
    openButtons(true);
    sign = 1;


    //![1]
        QBarSet *set0 = new QBarSet("Trafficflow");
        *set0 << 60 << 38 << 71 << 50 << 59 << 63 << 83 << 72 << 58 << 49 << 20 << 17;
    //![1]

    //![2]
        QBarSeries *series = new QBarSeries();
        series->append(set0);

    //![2]

    //![3]
        QChart *chart = new QChart();
        chart->addSeries(series);
        chart->setTitle("车辆流量");
        chart->setAnimationOptions(QChart::SeriesAnimations);
    //![3]

    //![4]
        QStringList categories;
        categories << "1-2" << "3-4" << "5-6" << "7-8" << "9-10" << "11-12" << "13-14" << "15-16"
                      << "17-18" << "19-20" << "21-22" << "23-24";
        QBarCategoryAxis *axis = new QBarCategoryAxis();
        axis->append(categories);
        chart->createDefaultAxes();
        chart->setAxisX(axis, series);
    //![4]

    //![5]
        chart->legend()->setVisible(true);
        chart->legend()->setAlignment(Qt::AlignBottom);
    //![5]

    //![6]
        QChartView *chartView = new QChartView(chart);
        chartView->setRenderHint(QPainter::Antialiasing);
    //![6]
/*
    //![7]
        QMainWindow window;
        window.setCentralWidget(chartView);
        window.resize(420, 300);
        window.show();



    //![1]
        QLineSeries *series = new QLineSeries();
    //![1]

    //![2]
        series->append(0, 6);
        series->append(2, 4);
        series->append(3, 8);
        series->append(7, 4);
        series->append(10, 5);
        *series << QPointF(11, 1) << QPointF(13, 3) << QPointF(17, 6) << QPointF(18, 3) << QPointF(20, 2);
    //![2]

    //![3]
        QChart *chart = new QChart();
        chart->legend()->hide();
        chart->addSeries(series);
        chart->createDefaultAxes();
        chart->setTitle("Simple line chart example");
    //![3]

    //![4]
        QChartView *chartView = new QChartView(chart);
        chartView->setRenderHint(QPainter::Antialiasing);
    //![4]
*/

    //![5]
    //!
        QGridLayout *baseLayout = new QGridLayout(); //便于显示，创建网格布局

        baseLayout->addWidget(chartView, 1, 0);
        ui->widgetShow->setLayout(baseLayout); //显示到QWidget控件

        ui->pushButtonDay->setEnabled(false);
}

void TrafficFlow::on_pushButtonMonth_clicked()
{
    openButtons(true);
    sign = 2;



    ui->pushButtonMonth->setEnabled(false);
}

void TrafficFlow::on_pushButtonYear_clicked()
{
    openButtons(true);
    sign = 3;



    ui->pushButtonYear->setEnabled(false);
}

void TrafficFlow::on_pushButtonFoward_clicked()
{

}

void TrafficFlow::on_pushButtonLater_clicked()
{

}

void TrafficFlow::on_toolButton_clicked()
{
    if(sign == 1)
    {
        on_pushButtonDay_clicked();
    }else if(sign == 2)
    {
        on_pushButtonMonth_clicked();
    }else
    {
        on_pushButtonYear_clicked();
    }
}
