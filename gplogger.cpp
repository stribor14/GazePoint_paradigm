#include "gplogger.h"

static QList<QByteArray> keys;

void GPLogger::startLog(const QString &folder, const QString &name)
{
    QString temp = folder + "/" + QDateTime::currentDateTime().toString("yy_MM_dd-hh_mm") + "__" + name;
    logFile.setFileName(temp + "_LOG.csv");
    eventFile.setFileName(temp + "_EVENT.csv");
    logFile.open(QIODevice::WriteOnly);
    eventFile.open(QIODevice::WriteOnly);
    logStream.setDevice(&logFile);
    eventStream.setDevice(&eventFile);

    for(auto &&key: keys)
        logStream << key.data() << ";" ;
    logStream << "\n";
    eventStream << "EVENT;EVENT_NUM;DATA_1;DATA_2;DATA_3;DATA_4;\n";

    isStarted = true;
}

void GPLogger::stopLog()
{
    isStarted = false;

    logFile.close();
    eventFile.close();
}

void GPLogger::logGaze(const QMap<QByteArray, double>  &data)
{
    if(!isStarted) return;
    for(auto &&key: keys)
        logStream << data.value(key) << ";" ;
    logStream << "\n";
    //qDebug() << data;
}

void GPLogger::logEvent(const QString &eventDescriptor, double eventNumber, double data1, double data2, double data3, double data4)
{
    //qDebug() << eventDescriptor.c_str() << ";" << eventNumber << ";" << data1 << ";" << data2 << ";" << data3 << ";" << data4;

    if(!isStarted) return;
    eventStream << eventDescriptor << ";" << eventNumber << ";" << data1 << ";" << data2 << ";" << data3 << ";" << data4 << ";\n" ;
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

    keys.append("LPCX");
    keys.append("LPCY");
    keys.append("LPD");
    keys.append("LPS");
    keys.append("LPV");
    keys.append("RPCX");
    keys.append("RPCY");
    keys.append("RPD");
    keys.append("RPS");
    keys.append("RPV");
}

QMap<QByteArray, double> GPDataParser::parseData(const QByteArray &data)
{
    QMap<QByteArray, double> temp;
    if(qstrcmp(data.mid(1,3),"REC")){
        temp["valid"] = 0;
    }
    else {
        temp["valid"] = 1;
        for(auto &&key: keys){
            int t_1 = data.indexOf(key, 0);
            int t_2 = data.indexOf('\"', t_1)+1;
            int t_3 = data.indexOf('\"', t_2);
            //qDebug() << key.c_str() << data.substr(t_2, t_3-t_2).c_str();
            temp[key] = data.mid(t_2, t_3-t_2).toDouble();
        }
    }

    return temp;
}
