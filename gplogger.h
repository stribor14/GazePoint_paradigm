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
    void startLog(QString folder, QString name);
    void stopLog();
    void logGaze(std::map<std::string, double> &data);
    void logEvent(int eventNumber, int gazeID, double gazeTime);

private:
    std::ofstream logFile;
    std::ofstream eventFile;
};


class GPDataParser
{
public:
    GPDataParser();
    std::map<std::string, double> parseData(std::string &data);

};
#endif // GPLOGGER_H
