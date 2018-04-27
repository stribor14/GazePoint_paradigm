#ifndef GPLOGGER_H
#define GPLOGGER_H

#include <QString>
#include <QDateTime>
#include <QDebug>
#include <QMap>
#include <QVector>

#include <fstream>
#include <string>

class GPLogger
{
public:
    GPLogger() = default;
    void startLog(const QString &folder, const QString &name);
    void stopLog();
    void logGaze(const QMap<QByteArray, double> &data);
    void logEvent(const std::string &eventDescriptor, double eventNumber, double data1, double data2, double data3 = 0, double data4 = 0);

private:
    std::ofstream logFile;
    std::ofstream eventFile;

    bool isStarted = false;
};


class GPDataParser
{
public:
    GPDataParser();
    QMap<QByteArray, double> parseData(const QByteArray &data);

};
#endif // GPLOGGER_H
