#include "gplogger.h"

GPLogger::GPLogger()
{

}

void GPLogger::startLog(QString folder, QString name)
{
    QString temp = folder + "" + QDateTime::currentDateTime().toString("yy_MM_dd-hh_mm") + "__" + name;
    logFile.open((temp + "_LOG.csv").toStdString(), std::ofstream::out | std::ofstream::app);
    eventFile.open((temp + "_EVENT.csv").toStdString(), std::ofstream::out | std::ofstream::app);
}

void GPLogger::stopLog()
{
    logFile.close();
}

void GPLogger::logGaze(std::deque<std::map<std::string, int> > &data)
{
    for(auto &&rec : data){
        for(auto &&atom: rec)
            logFile << atom.first << ";" << atom.second << ";" ;
        logFile << "\n";
    }

}

void GPLogger::logEvent(std::string &data)
{

}


////////////////////////////////////

// PODACI KOJI ĆE SE ŽVAKATI...

std::map<std::string, int> GPDataParser::parseData(std::string &data)
{
    std::map<std::string, int> temp;

    return temp;
}
