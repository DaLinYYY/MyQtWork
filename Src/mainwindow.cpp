#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "mytimer.h"
#include <QDebug>
#include <QtMath>
//#include <QtChart>

using namespace std;
int maxSize = 2000;
//#define MY_DEBUG



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)

    ,mySerialPort(new QSerialPort)
    ,mySerialInfo(0)

    ,timer(new QTimer)
    ,timerShow(new QTimer)
    ,newPortStringList(new mPortList)
    ,oldPortStringList(new mPortList)

    ,btnOpenIsOpen(false)
    ,btnSendIsSend(false)

    ,myrecvThread(0)
    ,mysendThread(0)
    ,recvBytes(0)
    ,sendBytes(0)

    ,count(0)


//    ,m_chartView(new ChartView)
    ,m_chart(new QChart)
    ,m_axisX(new QValueAxis())
    ,m_axisY(new QValueAxis())
    ,m_series(new QLineSeries)
{
    ui->setupUi(this);
    this->initUi();

    timer->setSingleShot(false);
    timer->start(COM_REFRESH_TIME);

    timerShow->setSingleShot(false);
    timerShow->start(UPDATE_SHOW_TIME);

    initSlot();
    initChartUi();


    /*固定窗口大小，不能最大化*/
    this->setFixedSize(this->size());

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initUi(void)
{
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        newPortStringList->ComName += info.portName();
        newPortStringList->Description += info.description();
        newPortStringList->Manufacturer += info.manufacturer();

//        foreach (const mPortList &info, newPortStringList)
//        oldPortStringList = newPortStringList;

        QSerialPort serialShow;
        serialShow.setPort(info);
        if (serialShow.open(QIODevice::ReadWrite))
        {

            ui->comPort->addItem(info.portName() + ": " + info.description());
            serialShow.close();
        }
    }
    for (int i = 0; i < newPortStringList->ComName.size(); i++)
    {
       oldPortStringList->ComName += newPortStringList->ComName[i];
       oldPortStringList->Description += newPortStringList->Description[i];
       oldPortStringList->Manufacturer += newPortStringList->Manufacturer[i];
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
    connect(timerShow, SIGNAL(timeout()), this, SLOT(updateData()));
}

void MainWindow::initChartUi()
{
//    m_chart = new QChart;
    chartView = new ChartView(m_chart);
    chartView->setRubberBand(QChartView::HorizontalRubberBand);


    series = new QLineSeries;
    m_chart->addSeries(series);


//    m_axisX->setGridLineVisible(false);
//    m_axisY->setGridLineVisible(true);

//    m_axisX->setRange(0, 100);
//    m_axisX->setTickCount(10);
//    m_chart->addAxis(m_axisX, Qt::AlignBottom);

//    m_axisY->setRange(-10, 10);
//    m_chart->addAxis(m_axisY, Qt::AlignLeft);

    m_chart->createDefaultAxes();


    m_chart->axisX()->setGridLineVisible(false);
    m_chart->axisY()->setGridLineVisible(true);

    m_chart->axisY()->setRange(-10, 10);
    m_chart->axisX()->setRange(0, 100);
    m_chart->legend()->hide();
    m_chart->setTheme(QChart::ChartThemeDark);


/************************first***************************/
#if 0
    m_series = new QLineSeries;
    m_series->setName("ADC1");
    for(int i=0;i<maxSize;++i){
       m_series->append(i,0.1);
    }
    m_series->setPen(QPen(Qt::red));
    m_series->setUseOpenGL(true);//openGl 加速
    qDebug()<<m_series->useOpenGL();
/*************************second**************************/

    series = new QLineSeries;
    series->setName("ADC2");
    for(int i=0;i<maxSize;++i){
       series->append(i,-0.1);
    }
    series->setPen(QPen(Qt::blue));

    series->setUseOpenGL(true);//openGl 加速
    qDebug()<<series->useOpenGL();
#endif
/*************************end**************************/


//    QValueAxis *axisX = new QValueAxis;
//    axisX->setTitleFont(QFont("Microsoft YaHei", 10, QFont::Normal, true));
//    axisX->setRange(0,100);
//    axisX->setTitleText("time/us");
//    m_chart->addAxis(axisX, Qt::AlignBottom);


//    QValueAxis *axisY = new QValueAxis;
//    axisY->setTitleFont(QFont("Microsoft YaHei", 10, QFont::Normal, true));
//    axisY->setRange(-10, 10);
//    axisY->setTitleText("Voltage/V");
//    m_chart->addAxis(axisY, Qt::AlignLeading);

//    m_chart->legend()->hide();

//    m_chart->createDefaultAxes();

    QVBoxLayout *layout = ui->verticalLayout;
    layout->addWidget(chartView);

}


void MainWindow::on_btnOpen_clicked()
{
    if(btnOpenIsOpen == false)
    {
        btnOpenIsOpen = true;
        ui->btnOpen->setText(tr("Close"));
//        qDebug()<< "Close";
//        qDebug() << ui->comPort->currentText().split(":")[0];
        mySerialInfo = new QSerialPortInfo(ui->comPort->currentText().split(":")[0]);
        mySerialPort->setPort(*mySerialInfo);
        mySerialPort->open(QIODevice::ReadWrite);




    }else{
        btnOpenIsOpen = false;
        ui->btnOpen->setText(tr("Open"));
//        qDebug() << "Open";
    }
}

void MainWindow::updateData()
{
    int i;
    QVector<QPointF> oldData = series->pointsVector();
    QVector<QPointF> data;

    if (oldData.size() < 300) {
        data = series->pointsVector();
    } else {

        for (i = 1; i < oldData.size(); ++i) {
            data.append(QPointF(i - 1 , oldData.at(i).y()));
        }
    }

    qint64 size = data.size();

    for(i = 0; i < 1; ++i){
        data.append(QPointF(i + size, 10 * sin(M_PI * count * 4 / 180)));
    }

    series->replace(data);
    count++;
}

/*Different item in the two list*/
mPortList* MainWindow::CheckPortListDif(mPortList *newList, mPortList *oldList)
{
    int len_new = newList->ComName.size();
    int len_old = oldList->ComName.size();
    mPortList* resList;
    bool addflag = false;

    if(len_new == 0)
    {
        resList = new mPortList[1];

        resList->PortChangeStatus = MINUS_TIEM_ALL;

        return resList;
    }

    if(len_old == 0)
    {
        resList = new mPortList[1];

        resList->PortChangeStatus = ADD_TIEM_ALL;

        return resList;
    }

    if(len_new > len_old)
    {
        resList = new mPortList[len_new - len_old];
        for(int i = 0; i < newList->ComName.size(); i++)
        {
            addflag = true;
            for(int j = 0; j < oldList->ComName.size(); j++)
            {
                if(newList->ComName[i] == oldList->ComName[j])
                    addflag = false;
            }
            if(addflag == true)
            {
                resList->ComName += newList->ComName[i];
                resList->Description += newList->Description[i];
                resList->Manufacturer += newList->Manufacturer[i];
                resList->PortChangeStatus = ADD_ITEM;
            }
        }
    }
    else
    {
        resList = new mPortList[len_old - len_new];
        for(int i = 0; i < oldList->ComName.size(); i++)
        {
            addflag = true;
            for(int j = 0; j < newList->ComName.size(); j++)
            {
                if(oldList->ComName[i] == newList->ComName[j])
                    addflag = false;
            }
            if(addflag == true)
            {
                resList->ComName += oldList->ComName[i];
                resList->Description += oldList->Description[i];
                resList->Manufacturer += oldList->Manufacturer[i];
                resList->PortChangeStatus = MINUS_ITEM;
            }
        }

    }
    return resList;
}

void MainWindow::wheelEvent(QWheelEvent *event)
{
    if (event->delta() > 0) {
        m_chart->zoom(1.1);
    } else {
        m_chart->zoom(10.0/11);
    }

    QWidget::wheelEvent(event);
}

/*Serial port automatically refreshes*/
void MainWindow::updateComShow()
{
//    qDebug() << "time test";
//    QSerialPort serialShow;
    mPortList * changeItme;

    newPortStringList->ComName.clear();
    newPortStringList->Description.clear();
    newPortStringList->Manufacturer.clear();

    /*Returns a list of available serial ports on the system.*/
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
//        qDebug() << "Name        : " << info.portName();
//        qDebug() << "Description : " << info.description();
//        qDebug() << "Manufacturer: " << info.manufacturer();

        newPortStringList->ComName += info.portName();
        newPortStringList->Description += info.description();
        newPortStringList->Manufacturer += info.manufacturer();
    }

    /*If different, Flush number of serial channel caches*/
    if(newPortStringList->ComName.size() != oldPortStringList->ComName.size())
    {
        changeItme = new mPortList[abs(newPortStringList->ComName.size() - oldPortStringList->ComName.size())];

        changeItme = CheckPortListDif(newPortStringList, oldPortStringList);

//        qDebug() << changeItme->PortChangeStatus;

        switch (changeItme->PortChangeStatus)
        {
        case ADD_TIEM_ALL :{

            ui->comPort->clear();
            for (int i = 0; i < newPortStringList->ComName.size(); i++)
            {
                ui->comPort->addItem(newPortStringList->ComName[i] + ": " + newPortStringList->Description[i]);
            }
            break;
        }
        case MINUS_TIEM_ALL:{

            ui->comPort->clear();

            break;

        }
        case ADD_ITEM:{

            for( int i = 0; i < changeItme->ComName.size(); i++)
            {
                ui->comPort->addItem(changeItme->ComName[i] + ": " + changeItme->Description[i]);
            }

            break;

        }
        case MINUS_ITEM:{

            for(int i = 0; i < changeItme->ComName.size(); i++)
            {
                QString cStr = changeItme->ComName[i] + ": " + changeItme->Description[i];

                for(int i = 0; i < ui->comPort->count(); i++)
                {
                    if(cStr == ui->comPort->itemText(i))
                        ui->comPort->removeItem(i);
                }
            }

            break;
        }
        default:
            break;

        }

        for(int i = 0; i < ui->comPort->count(); i++)
        {
            qDebug() << ui->comPort->itemText(i);
        }
        qDebug() << "";


        oldPortStringList->ComName.clear();
        oldPortStringList->Description.clear();
        oldPortStringList->Manufacturer.clear();

        for (int i = 0; i < newPortStringList->ComName.size(); i++)
        {
           oldPortStringList->ComName += newPortStringList->ComName[i];
           oldPortStringList->Description += newPortStringList->Description[i];
           oldPortStringList->Manufacturer += newPortStringList->Manufacturer[i];
        }

    }
}


