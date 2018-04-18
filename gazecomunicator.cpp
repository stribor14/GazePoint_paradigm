#include "gazecomunicator.h"

GazeComunicator::GazeComunicator(QObject * parent) : QObject(parent)
{
    GP = new GPClient();
    moveToThread(&thread);
    connect(&thread, SIGNAL(started()), this, SLOT(MsgLoop()));
}

GazeComunicator::~GazeComunicator()
{
    delete GP;
}

void GazeComunicator::Start()
{
    GP->client_connect();
    while(!GP->is_connected());
    GP->send_cmd("<SET ID=\"ENABLE_SEND_TIME\" STATE=\"1\" />");
    GP->send_cmd("<SET ID=\"ENABLE_SEND_POG_FIX\" STATE=\"1\" />");
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

void GazeComunicator::MsgLoop()
{
    deque<string> buffer;
    while(!stopThread){
        GP->get_rx(buffer);
        if(buffer.size() > 0)
        {
          qDebug() << buffer.size();
        }
    }
    QThread::currentThread()->quit();
}
