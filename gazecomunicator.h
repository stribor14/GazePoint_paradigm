#ifndef GAZECOMUNICATOR_H
#define GAZECOMUNICATOR_H

#include <QObject>
#include <QDebug>

#include <string>

#include "qgpclient.h"
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

    QGPClient* GP;
    GPLogger* logger;
    GPDataParser* parser;
public:
    GazeComunicator(QObject * parent = NULL);
    ~GazeComunicator();
    void Start();
    void Stop();
    void startLog(const QString &folder, const QString &name);
    void stopLog();

    void logCustomEvent(const std::string &eventDescriptor, double eventNumber, double data1, double data2, double data3 = 0, double data4 = 0);
    void setPerimeter(const double &perimeterX, const double &perimeterY, const bool &needUnlock = false);
    void setTarget(const int &num, const double &x, const double &y);

public slots:
    void msgProcessing(const QByteArray &msg);

signals:
    void targetReached();
};

#endif // GAZECOMUNICATOR_H
