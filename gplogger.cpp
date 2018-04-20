#include "gplogger.h"

QList<std::string> keys;

GPLogger::GPLogger()
{

}

void GPLogger::startLog(const QString &folder, const QString &name)
{
    QString temp = folder + "" + QDateTime::currentDateTime().toString("yy_MM_dd-hh_mm") + "__" + name;
    logFile.open((temp + "_LOG.csv").toStdString(), std::ofstream::out | std::ofstream::app);
    eventFile.open((temp + "_EVENT.csv").toStdString(), std::ofstream::out | std::ofstream::app);

    for(auto &&key: keys)
        logFile << key << ";" ;
    logFile << "\n";
    eventFile << "TARGET_NUM;GAZE_ID;GAZE_TIME;\n";
}

void GPLogger::stopLog()
{
    logFile.close();
    eventFile.close();
}

void GPLogger::logGaze(const std::map<std::string, double> &data)
{
    for(auto &&key: keys)
        logFile << data.at(key) << ";" ;
    logFile << "\n";
}

void GPLogger::logEvent(const int &eventNumber, const int &gazeID, const double &gazeTime)
{
    eventFile << eventNumber << ";" << gazeID << ";" << gazeTime << "\n" ;
    qDebug() << eventNumber << ";" << gazeID << ";" << gazeTime;
}


////////////////////////////////////

// PODACI KOJI ĆE SE ŽVAKATI...

GPDataParser::GPDataParser()
{
    keys.append("CNT");
    keys.append("TIME");
    keys.append("FPOGX");
    keys.append("FPOGY");
    keys.append("FPOGS");
    keys.append("FPOGD");
    keys.append("FPOGID");
    keys.append("FPOGV");
    keys.append("BPOGX");
    keys.append("BPOGY");
    keys.append("BPOGV");
}

std::map<std::string, double> GPDataParser::parseData(const std::string &data)
{
    std::map<std::string, double> temp;
    if(data.substr(1,3).compare("REC")){
        temp["valid"] = 0;
    }
    else {
        temp["valid"] = 1;
        for(auto &&key: keys){
            size_t t_1 = data.find(key);
            size_t t_2 = data.find("\"", t_1)+1;
            size_t t_3 = data.find("\"", t_2);
            //qDebug() << key.c_str() << data.substr(t_2, t_3-t_2).c_str();
            temp[key] = std::stod(data.substr(t_2, t_3-t_2).c_str());
        }
    }

    return temp;
}
