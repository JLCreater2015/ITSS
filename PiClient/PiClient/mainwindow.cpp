#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <wiringPi.h>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_tcpsocket = new QTcpSocket(this);
    init();
    on_pushButton_clicked();
    wiringPiSetup();
    pinMode(27,OUTPUT);
    isCome = true;
}

MainWindow::~MainWindow()
{
    if(m_tcpsocket)
        delete m_tcpsocket;
    if(myCom)
        delete myCom;
    if(timer)
        delete timer;
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{

    myCom = new Posix_QextSerialPort("/dev/ttyACM0",QextSerialBase::Polling);


    //define the member of serial ,and sen the number ,declear for it in the constructor

    myCom->setBaudRate(BAUD9600); //波特率设置，我们设置为9600,这个值是我在用MSP430传数时用的最合适的波特率。

    myCom->setDataBits(DATA_8);  //数据位设置，我们设置为8位数据位
    myCom->setParity(PAR_NONE);  //奇偶校验设置，我们设置为无校验
    myCom->setStopBits(STOP_1);  //停止位设置，我们设置为1位停止位
    myCom->setFlowControl(FLOW_OFF); //数据流控制设置，我们设置为无数据流控制

    myCom->open(QIODevice::ReadWrite);  //open the serial with the style of ReadWrite.
     //connect(myCom,SIGNAL(readyRead()),this,SLOT(readMyCom()));   //这个为中断方式，但linux下只能用查询方式
     //conect the SLOT and SIGNAL,when there are data in the serial buffer,it will read the serial


    //开启读取定时器
    timer->start(timerdly);
    //设置数据流控制
    //myCom->setFlowControl(FLOW_OFF);
    //设置延时
    myCom->setTimeout(TIME_OUT);
    ui->statusBar->showMessage(tr("Open ttyACM0 Successfully!"));

    ui->pushButton->setEnabled(false);
    ui->pushButton_2->setEnabled(true);
}

void MainWindow::on_pushButton_2_clicked()
{
    myCom->close();
    delete myCom;
    timer->stop();
    ui->pushButton->setEnabled(true);
    ui->pushButton_2->setEnabled(false);
    ui->statusBar->showMessage(tr("ttyACM0 has already been closed!"));
}

void MainWindow::on_pushButton_3_clicked()
{
    //m_tcpsocket->abort();
    QString str = "1#";
    QString carID = ui->lineEdit->text();
    QByteArray message;
    if(isCome)
        str = "2#comeIn#";
    else
        str = "2#goOut#";
    isCome=!isCome;
    m_tcpsocket->connectToHost(QHostAddress("192.168.43.189"),6666);//设置客户端的端口号
    if(m_tcpsocket->error())
    {
        qDebug()<<m_tcpsocket->errorString();
    }
    str += carID;
    message=str.toUtf8();
    qDebug()<<message;
    m_tcpsocket->waitForConnected(1000);
    m_tcpsocket->write(message);
}
void MainWindow::setButtonEnabled(bool status)
{
    ui->pushButton->setEnabled(status);
    ui->pushButton_2->setEnabled(status);
    ui->pushButton_3->setEnabled(status);
}

void MainWindow::init()
{
    setButtonEnabled(false);
    ui->pushButton_2->setEnabled(false);
    //初始化读取定时器计时间隔
    timerdly = TIMER_INTERVAL;

    //设置读取计时器
    timer = new QTimer(this);

    //设置等待定时器
    writtimer = new QTimer(this);
    //建立定时读串口连接
    connect(timer, SIGNAL(timeout()), this, SLOT(readMyCom()));
    connect(writtimer,SIGNAL(timeout()),this, SLOT(closeDJ()));
   //connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(pushButtonclicked()));
    //connect(ui->pushButtonopen, SIGNAL(clicked()), this, SLOT(pushButtonopenclicked()));
    //connect(ui->pushButtonclose, SIGNAL(clicked()), this, SLOT(pushButtoncloseclicked()));
    setButtonEnabled(true);
}
int hexTOdec(char b)
{
    if(b >= '0'&&b <='9')//数字字符的转换
    {
        return b-48;
    }
    else if(b>='A'&&b<='F')//字母字符的转换
    {
        return b-55;
    }
    else if(b>='a'&&b<='f')//字母字符的转换
    {
        return b-87;
    }
    else
        return 0;
}
void MainWindow::readMyCom()  //The function for read
{
    QByteArray carID = myCom->readAll().toHex();


    //read all data int the buffer ,and send it to the temporary variable "temp"

    //temp=temp.toHex();

    //show the data of serial int the texBrowser

    carID.remove(0,2);
    //qint64 len = myCom->readLine(myData, 20);
    if(!carID.isEmpty())
    {
        QString qStr(carID);
        QByteArray ba = qStr.toLatin1();
        char *a = ba.data();
        qDebug()<<a;
        qlonglong sum=0,i=0;
        while(i<carID.length())
        {
            sum = sum * 16 + hexTOdec(a[i]);
            i++;
        }
        qDebug()<<sum;
        QString ss,sss;
        while(sum>0)
        {
            QChar a = (QChar)(48 + (int)(sum % 10));
            sum = sum / 10;
            ss.append(a);
        }
        for(int i = ss.length() - 1;i >= 0;i--)
        {
            sss.append(ss[i]);
        }
        ui->lineEdit->setText(sss);
        //m_tcpsocket->abort();
        QString str = "1#";
        QString carID = ui->lineEdit->text();
        QByteArray message;
        m_tcpsocket->connectToHost(QHostAddress("192.168.43.189"),6666);//设置客户端的端口号
        if(m_tcpsocket->error())
        {
            qDebug()<<m_tcpsocket->errorString();
        }
        str += carID;
        message=str.toUtf8();
        qDebug()<<message;
        //等待连接后发送
        m_tcpsocket->waitForConnected(1000);

        m_tcpsocket->write(message);

        //openDJ(120);
        digitalWrite(27,HIGH);
        delayMicroseconds(1500);
        digitalWrite(27,LOW);
        delayMicroseconds(18500);
        writtimer->start(5000);
    }
}

void MainWindow::closeDJ()
{
    writtimer->stop();
    //openDJ(0);
    digitalWrite(27,HIGH);
    delayMicroseconds(500);
    digitalWrite(27,LOW);
    delayMicroseconds(19500);
}

void MainWindow::openDJ(int angle)
{
    int i=0; float x=0;
    int k=180;  //最多转动180读
    while(k--)
    {
        x=11.11*i;    //每转一度用时11.11微妙，为了降低速度实验测得的数据
        digitalWrite(27,HIGH);
        delayMicroseconds(500+x);
        digitalWrite(27,LOW);
        delayMicroseconds(19500-x);
        if(i == angle)    //旋转angle角度后停止
            break;
        i++;
    }
}

