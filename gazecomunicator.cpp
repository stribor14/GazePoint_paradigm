#include "gazecomunicator.h"

static int tempN = 0;

GazeComunicator::GazeComunicator(QObject * parent) : QObject(parent)
{
    GP = new QGPClient();
    logger = new GPLogger();
    parser = new GPDataParser();

    connect(GP, &QGPClient::msgReceived, this, &GazeComunicator::msgProcessing);
}

GazeComunicator::~GazeComunicator()
{
    delete GP;
    delete parser;
    delete logger;
}

void GazeComunicator::Start()
{
    if(!GP->clientConnect()) return;
    GP->sendCmd(R"(<SET ID="ENABLE_SEND_TIME" STATE="1" />)");
    GP->sendCmd(R"(<SET ID="ENABLE_SEND_COUNTER" STATE="1" />)");
    GP->sendCmd(R"(<SET ID="ENABLE_SEND_POG_FIX" STATE="1" />)");
    GP->sendCmd(R"(<SET ID="ENABLE_SEND_POG_BEST" STATE="1" />)");

    GP->sendCmd(R"(<SET ID="ENABLE_SEND_DATA" STATE="1" />)");
}

void GazeComunicator::Stop()
{
    if(GP->getState() == QTcpSocket::ConnectedState)
        GP->clientDisconnect();
}

void GazeComunicator::startLog(const QString &folder, const QString &name)
{
    logger->startLog(folder, name);
}

void GazeComunicator::stopLog()
{
    logger->stopLog();
}

void GazeComunicator::logCustomEvent(const std::string &eventDescriptor, double eventNumber, double data1, double data2, double data3, double data4)
{
    logger->logEvent(eventDescriptor, eventNumber, data1, data2, data3, data4);
}

void GazeComunicator::setPerimeter(const double &perimeterX, const double &perimeterY, const bool &needUnlock)
{
     targetPerimeterX = perimeterX;
     targetPerimeterY = perimeterY;
     targetUnlock = needUnlock;
}

void GazeComunicator::setTarget(const int &num, const double &x, const double &y)
{
    targetNum = num;
    targetX = x;
    targetY = y;
    targetPending = true;
    qDebug() << x << y << num << targetPending;
}

void GazeComunicator::msgProcessing(const QByteArray &msg)
{
    if(msg.at(0) != '<') return; // if data doesnt start with '<'
    QMap<QByteArray, double>  temp = parser->parseData(msg);
    if(temp.value("valid") == 0) return;
if( temp.value("CNT") - tempN > 1 ) qDebug() << temp.value("CNT") - tempN;
tempN = temp.value("CNT");
    logger->logGaze(temp);
    if(targetPending){
        if(fabs(temp.value("BPOGX") - targetX) < targetPerimeterX && fabs(temp.value("BPOGY") - targetY) < targetPerimeterY){
            targetPending = false;
            logger->logEvent("DOT_GAZED", targetNum, temp.value("CNT"), temp.value("TIME"), targetX, targetY);
            if(targetUnlock) emit targetReached();
        }
    }
}
