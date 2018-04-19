#include "gazecomunicator.h"

QMutex GPmutex;

GazeComunicator::GazeComunicator(QObject * parent) : QObject(parent)
{
    GP = new GPClient();
    logger = new GPLogger();
    parser = new GPDataParser();
    moveToThread(&thread);
    connect(&thread, SIGNAL(started()), this, SLOT(MsgLoop()));
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
    GP->send_cmd("<SET ID=\"ENABLE_SEND_TIME\" STATE=\"1\" />");
    GP->send_cmd("<SET ID=\"ENABLE_SEND_COUNTER\" STATE=\"1\" />");
    GP->send_cmd("<SET ID=\"ENABLE_SEND_POG_FIX\" STATE=\"1\" />");
    GP->send_cmd("<SET ID=\"ENABLE_SEND_POG_BEST\" STATE=\"1\" />");

    // zjenica?

    GP->send_cmd("<SET ID=\"ENABLE_SEND_DATA\" STATE=\"1\" />");

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

void GazeComunicator::startLog(QString folder, QString name)
{
    logger->startLog(folder, name);
}

void GazeComunicator::stopLog()
{
    logger->stopLog();
}

void GazeComunicator::setPerimeter(double perimeterX, double perimeterY)
{
     targetPerimeterX = perimeterX;
     targetPerimeterY = perimeterY;
}

void GazeComunicator::setTarget(int num, double &x, double &y, bool needUnlock)
{
    GPmutex.lock();
    targetNum = num;
    targetX = x;
    targetY = y;
    targetUnlock = needUnlock;
    targetFound = false;
    GPmutex.unlock();
}

void GazeComunicator::MsgLoop()
{
    deque<string> buffer;
    while(!stopThread){
        GP->get_rx(buffer);
        for(auto &&data: buffer){
            std::map<std::string, double> temp = std::move(parser->parseData(data));
            logger->logGaze(temp);
            if(!targetFound)
                if(fabs(temp["BPOGX"] - targetX) < targetPerimeterX && fabs(temp["BPOGY"] - targetY) < targetPerimeterY){
                    targetFound = true;
                    logger->logEvent(targetNum, temp["CNT"], temp["TIME"]);
                    if(targetUnlock) emit targetReached();
                }
        }

    }
    QThread::currentThread()->quit();
}
