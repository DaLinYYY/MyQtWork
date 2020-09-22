#ifndef RECVTHREAD_H
#define RECVTHREAD_H

#include <QThread>
#include <QMutex>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>

class recvthread : public QThread
{
    Q_OBJECT
public:
    explicit recvthread(QObject *parent = nullptr);
    ~recvthread();

    void startRecv(QSerialPort *port, int waitTimeout);

public slots:
    void read_dataSlot();
    void write_data(int value);


private:
    int waiTimeout;

};

#endif // RECVTHREAD_H
