#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "mytimer.h"
#include <QMainWindow>
#include <QTimer>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>

#define UPDATE_SHOW_TIME   1000

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
    mPortList* CheckPortListDif(mPortList *newList, mPortList *oldList);



private:
    Ui::MainWindow *ui;
    QSerialPort *mySerialPort;
    bool btnOpenIsOpen;
    QTimer *timer;
    mPortList *newPortStringList;
    mPortList *oldPortStringList;

private slots:
    void on_btnOpen_clicked();
    void updateComShow();
};
#endif // MAINWINDOW_H
