#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "mytimer.h"
#include <QDebug>

using namespace std;

//#define MY_DEBUG



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    ,mySerialPort(new QSerialPort)
    ,btnOpenIsOpen(false)
    ,timer(new QTimer)
    ,newPortStringList(new mPortList)
    ,oldPortStringList(new mPortList)
{
    ui->setupUi(this);
    this->initUi();

    timer->setSingleShot(false);
    timer->start(UPDATE_SHOW_TIME);

    initSlot();


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initUi(void)
{
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
#ifdef MY_DEBUG
        qDebug() << "Name        : " << info.portName();
        qDebug() << "Description : " << info.description();
        qDebug() << "Manufacturer: " << info.manufacturer();

//        qDebug() << info.serialNumber();
//        qDebug() << info.systemLocation();
//        qDebug() << info.vendorIdentifier();
//        qDebug() << info.productIdentifier();
//        qDebug() << info.availablePorts();
#endif
        newPortStringList->ComName += info.portName();
        newPortStringList->Description += info.description();
        newPortStringList->Manufacturer += info.manufacturer();

//        foreach (const mPortList &info, newPortStringList)
        for (int i = 0; i < newPortStringList->ComName.size(); i++)
        {
           oldPortStringList->ComName += newPortStringList->ComName[i];
           oldPortStringList->Description += newPortStringList->Description[i];
           oldPortStringList->Manufacturer += newPortStringList->Manufacturer[i];
        }
//        oldPortStringList = newPortStringList;

        QSerialPort serialShow;
        serialShow.setPort(info);
        if (serialShow.open(QIODevice::ReadWrite))
        {

            ui->comPort->addItem(info.portName() + ": " + info.description());
            serialShow.close();
        }
    }
    mySerialPort->setBaudRate(QSerialPort::Baud9600);
    mySerialPort->setDataBits(QSerialPort::Data8);
    mySerialPort->setStopBits(QSerialPort::OneStop);
    mySerialPort->setParity(QSerialPort::NoParity);
    mySerialPort->setFlowControl(QSerialPort::NoFlowControl);
}

void MainWindow::initSlot()
{
    connect(timer, SIGNAL(timeout()), this, SLOT(updateComShow()));
}


void MainWindow::on_btnOpen_clicked()
{
//    if(btnOpenIsOpen == false)
//    {
//        btnOpenIsOpen = true;
//        qDebug() << "false";
//    }else{
//        btnOpenIsOpen = false;
//        qDebug() << "True";
//    }

    if(btnOpenIsOpen == false)
    {
        btnOpenIsOpen = true;
        ui->btnOpen->setText(tr("Close"));
        qDebug()<< "Close";

    }else{
        btnOpenIsOpen = false;
        ui->btnOpen->setText(tr("Open"));
        qDebug() << "Open";
    }
}

void MainWindow::updateComShow()
{
//    qDebug() << "time test";
//    QSerialPort serialShow;
    newPortStringList->ComName.clear();
    newPortStringList->Description.clear();
    newPortStringList->Manufacturer.clear();
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
//        qDebug() << "Name        : " << info.portName();
//        qDebug() << "Description : " << info.description();
//        qDebug() << "Manufacturer: " << info.manufacturer();

        newPortStringList->ComName += info.portName();
        newPortStringList->Description += info.description();
        newPortStringList->Manufacturer += info.manufacturer();
    }


    if(newPortStringList->ComName.size() != oldPortStringList->ComName.size())
    {
        oldPortStringList->ComName.clear();
        oldPortStringList->Description.clear();
        oldPortStringList->Manufacturer.clear();

        #ifdef MY_DEBUG
//            qDebug() << "";
//            qDebug() << &newPortStringList->ComName;
//            qDebug() << &oldPortStringList->ComName;

//            qDebug() << "Name        : " << newPortStringList->ComName;
//            qDebug() << "Description : " << newPortStringList->Description;
//            qDebug() << "Manufacturer: " << newPortStringList->Manufacturer;
        #endif

        for (int i = 0; i < newPortStringList->ComName.size(); i++)
        {
           oldPortStringList->ComName += newPortStringList->ComName[i];
           oldPortStringList->Description += newPortStringList->Description[i];
           oldPortStringList->Manufacturer += newPortStringList->Manufacturer[i];
        }
        ui->comPort->clear();

        for (int i = 0; i < oldPortStringList->ComName.size(); i++)
        {
            ui->comPort->addItem(newPortStringList->ComName[i] + ": " + newPortStringList->Description[i]);
        }
    }
}
