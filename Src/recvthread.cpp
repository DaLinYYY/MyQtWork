#include "recvthread.h"
#include <QDebug>

recvthread::recvthread(QObject *parent) : QThread(parent)
{

}

recvthread::~recvthread()
{

}

void recvthread::startRecv(QSerialPort *port, int waitTimeout)
{
    qDebug() << port->size();
    qDebug() << waitTimeout ;

}

void recvthread::read_dataSlot()
{

}

void recvthread::write_data(int value)
{
    qDebug() << value;

}
