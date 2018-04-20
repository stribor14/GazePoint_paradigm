#ifndef GAZECOMUNICATOR_H
#define GAZECOMUNICATOR_H

#include <QObject>
#include <QDebug>
#include <QThread>
#include <QMutex>
#include <QEventLoop>

#include <utility>

#include "GPClient.h"
#include "gplogger.h"

class GazeComunicator : public QObject
{
    Q_OBJECT
private:
    int targetNum;
    int customEventNum;
    double targetX;
    double targetY;
    double targetPerimeterX;
    double targetPerimeterY;
    bool targetUnlock;

    bool targetPending = false;
    bool customEventPending = false;

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
    void startLog(const QString &folder, const QString &name);
    void stopLog();

    void logCustomEvent(const int &eventNum);
    void setPerimeter(const double &perimeterX, const double &perimeterY, const bool &needUnlock = false);
    void setTarget(const int &num, const double &x, const double &y);

public slots:
    void MsgLoop();

signals:
    void targetReached();
};

#endif // GAZECOMUNICATOR_H
