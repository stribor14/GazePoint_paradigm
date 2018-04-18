#ifndef GAZECOMUNICATOR_H
#define GAZECOMUNICATOR_H

#include "GPClient.h"

#include <QObject>
#include <QDebug>
#include <QThread>


class GazeComunicator : QObject
{
    Q_OBJECT
public:
    GazeComunicator(QObject * parent = NULL);
    ~GazeComunicator();
    void Start();
    void Stop();

private slots:
    void MsgLoop();

signals:
    void sMsgArrived();

private:
    GPClient* GP;
    QThread thread;
    bool stopThread;
};

#endif // GAZECOMUNICATOR_H
