#ifndef MYTIMER_H
#define MYTIMER_H

#include <QObject>
#include <QTimer>
#include "mainwindow.h"

#define UPDATE_SHOW_TIME   1000

class mytimer : public QObject
{
    Q_OBJECT
public:
    explicit mytimer(QObject *parent = nullptr);
    void myTimerInit(void);
private:
    QTimer *m_timer;

private slots:
    void updateComShow(void);

};

#endif // MYTIMER_H
