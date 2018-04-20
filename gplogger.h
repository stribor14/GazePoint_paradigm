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
    GPLogger();
    void startLog(const QString &folder, const QString &name);
    void stopLog();
    void logGaze(const std::map<std::string, double> &data);
    void logEvent(const std::string &eventDescriptor, const double &eventNumber, const double &data1, const double &data2);

private:
    std::ofstream logFile;
    std::ofstream eventFile;
};


class GPDataParser
{
public:
    GPDataParser();
    std::map<std::string, double> parseData(const std::string &data);

};
#endif // GPLOGGER_H
