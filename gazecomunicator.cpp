#include "gazecomunicator.h"

GazeComunicator::GazeComunicator(QObject * parent) : QObject(parent)
{
    GP = new GPClient();
    logger = new GPLogger();
    parser = new GPDataParser();
    moveToThread(&thread);
    connect(&thread, &QThread::started, this, &GazeComunicator::MsgLoop);
}

GazeComunicator::~GazeComunicator()
{
    delete GP;
    delete parser;
    delete logger;
}

void GazeComunicator::Start()
{
    GP->client_connect();
    while(!GP->is_connected());
    GP->send_cmd(R"(<SET ID="ENABLE_SEND_TIME" STATE="1" />)");
    GP->send_cmd(R"(<SET ID="ENABLE_SEND_COUNTER" STATE="1" />)");
    GP->send_cmd(R"(<SET ID="ENABLE_SEND_POG_FIX" STATE="1" />)");
    GP->send_cmd(R"(<SET ID="ENABLE_SEND_POG_BEST" STATE="1" />)");

    // zjenica?

    GP->send_cmd(R"(<SET ID="ENABLE_SEND_DATA" STATE="1" />)");

    stopThread = false;
    thread.start();
}

void GazeComunicator::Stop()
{
    GP->client_disconnect();
    while(GP->is_connected());

    stopThread = true;
    thread.wait();      // if you want synchronous stop
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

void GazeComunicator::MsgLoop()
{
    deque<string> buffer;
    while(!stopThread){
        GP->get_rx(buffer);
        for(auto &&data: buffer){
            if(data.at(0) != '<') break; // if data doesnt start with '<'
            std::map<std::string, double> temp = parser->parseData(data);
            if(temp["valid"] == 0) continue;
            logger->logGaze(temp);
            if(targetPending){
                if(fabs(temp["BPOGX"] - targetX) < targetPerimeterX && fabs(temp["BPOGY"] - targetY) < targetPerimeterY){
                    targetPending = false;
                    logger->logEvent("DOT_GAZED", targetNum, temp["CNT"], temp["TIME"], targetX, targetY);
                    if(targetUnlock) emit targetReached();
                }
            }
        }

    }
    QThread::currentThread()->quit();
}
