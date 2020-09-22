#include "mytimer.h"
#include <QtDebug>
#include "mainwindow.h"
using namespace std;

mytimer::mytimer(QObject *parent) : QObject(parent)
{
    myTimerInit();
}

void mytimer::myTimerInit()
{
    if (m_timer == nullptr)
    {
        m_timer = new QTimer;
    }
    m_timer->setSingleShot(false);

    m_timer->start(UPDATE_SHOW_TIME);

    connect(m_timer, SIGNAL(timeout()), this, SLOT(updateComShow()));
}

void mytimer::updateComShow()
{
    qDebug() << "timer test";


}
