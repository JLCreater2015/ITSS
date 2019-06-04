#include "welcome.h"
#include "ui_welcome.h"
#include <QMovie>

Welcome::Welcome(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Welcome)
{
    ui->setupUi(this);
    setWindowFlags(Qt::CustomizeWindowHint|Qt::FramelessWindowHint);

    QMovie *movie = new QMovie("D:\\Qt\\Projects\\ITSS\\images\\bg.jpg");
    ui->label->setMovie(movie);
    movie->start();
    ui->label->setMargin(0);
}

Welcome::~Welcome()
{
    delete ui;
}
