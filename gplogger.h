#ifndef GPLOGGER_H
#define GPLOGGER_H

#include <QString>
#include <QDateTime>
#include <QMap>
#include <QVector>

#include <fstream>
#include <deque>
#include <string>

class GPLogger
{
public:
    GPLogger();
    void startLog(QString folder, QString name);
    void stopLog();
    void logGaze(std::deque<std::map<std::string, int>> &data);
    void logEvent(std::string &data);

private:
    std::ofstream logFile;
    std::ofstream eventFile;
};


class GPDataParser
{
public:
    GPDataParser();
    std::map<std::string, int> parseData(std::string &data);
};
#endif // GPLOGGER_H
