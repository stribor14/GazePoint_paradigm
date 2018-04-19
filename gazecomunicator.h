#ifndef GAZECOMUNICATOR_H
#define GAZECOMUNICATOR_H

#include "GPClient.h"
#include "gplogger.h"

#include <QObject>
#include <QDebug>
#include <QThread>
#include <QMutex>
#include <QEventLoop>

#include <utility>


class GazeComunicator : public QObject
{
    Q_OBJECT
private:
    int targetNum;
    double targetX;
    double targetY;
    double targetPerimeterX;
    double targetPerimeterY;
    bool targetUnlock;
    bool targetFound = true;

    GPClient* GP;
    GPLogger* logger;
    GPDataParser* parser;
    QThread thread;
    bool stopThread;

public:
    GazeComunicator(QObject * parent = NULL);
    ~GazeComunicator();
    void Start();
    void Stop();
    void startLog(QString folder, QString name);
    void stopLog();

    void setPerimeter(double perimeterX, double perimeterY);
    void setTarget(int num, double &x, double &y, bool needUnlock = false);

public slots:
    void MsgLoop();

signals:
    void targetReached();
};

#endif // GAZECOMUNICATOR_H
