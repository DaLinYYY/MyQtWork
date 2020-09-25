#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "mytimer.h"
#include "chartview.h"

#include <QMainWindow>
#include "recvthread.h"
#include <QTimer>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QtCharts/QChart>
#include <QtCharts/QDateTimeAxis>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>

QT_CHARTS_USE_NAMESPACE

#define UPDATE_SHOW_TIME   10
#define COM_REFRESH_TIME   100

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

enum PORT_CHANGE_STATUS{
    NOT_CHANGE,
    ADD_ITEM,
    MINUS_ITEM,
    ADD_TIEM_ALL,
    MINUS_TIEM_ALL
};

typedef  struct{
    QStringList ComName;
    QStringList Description;
    QStringList Manufacturer;
    int         PortChangeStatus;

}mPortList;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


private:
    void initUi();
    void initSlot();
    void initChartUi();
    mPortList* CheckPortListDif(mPortList *newList, mPortList *oldList);
    void wheelEvent(QWheelEvent *event);



private:
    Ui::MainWindow *ui;
    QSerialPort *mySerialPort;
    QSerialPortInfo *mySerialInfo;


    QTimer *timer;
    QTimer *timerShow;
    mPortList *newPortStringList;
    mPortList *oldPortStringList;

    bool btnOpenIsOpen;
    bool btnSendIsSend;
    recvthread * myrecvThread;
    recvthread * mysendThread;
    unsigned int recvBytes;
    unsigned int sendBytes;

    quint16 count;

//    ChartView *chartView;
    ChartView *chartView;
    QChart *m_chart;
    QValueAxis *m_axisX;
    QValueAxis *m_axisY;
    QLineSeries *m_series,*series;

private slots:
    void on_btnOpen_clicked();
    void updateComShow();
    void updateData();
};
#endif // MAINWINDOW_H
