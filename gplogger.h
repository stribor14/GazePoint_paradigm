#ifndef GPLOGGER_H
#define GPLOGGER_H

#include <QString>
#include <QDateTime>
#include <QDebug>
#include <QMap>
#include <QVector>
#include <QFile>

class GPLogger
{
public:
    GPLogger() = default;
    void startLog(const QString &folder, const QString &name);
    void stopLog();
    void logGaze(const QMap<QByteArray, double> &data);
    void logEvent(const QString &eventDescriptor, double eventNumber, double data1, double data2, double data3 = 0, double data4 = 0);

private:
    QFile logFile;
    QFile eventFile;
    QTextStream logStream;
    QTextStream eventStream;

    bool isStarted = false;
};


class GPDataParser
{
public:
    GPDataParser();
    QMap<QByteArray, double> parseData(const QByteArray &data);

};
#endif // GPLOGGER_H
