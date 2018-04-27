#include "qgpclient.h"

void QGPClient::readData()
{
    int delimiter_index;
    QByteArray data = tcpSocket->readAll();
    delimiter_index = data.indexOf("\r\n", 0);
    while (data.size())
    {
        QByteArray tmp = data.mid(0, delimiter_index);

        dataMutex.lock();
            dataBuffer.append(tmp);
            while (dataBuffer.size() > bufferSize)
            {
                dataBuffer.removeFirst();
            }
        dataMutex.unlock();

        data.remove(0, delimiter_index+2);
        delimiter_index = data.indexOf("\r\n", 0);
    }
}

QGPClient::QGPClient(QObject * parent) : QObject(parent),
    tcpSocket(new QTcpSocket())
{
    ipPort = 4242;
    ipAddress = "127.0.0.1";

    bufferSize = 60*60*3;
    connect(tcpSocket, &QTcpSocket::readyRead, this, &QGPClient::readData);
}

bool QGPClient::clientConnect()
{
    tcpSocket->connectToHost(ipAddress, ipPort);
    if (!tcpSocket->waitForConnected(1000)) return false;
    return true;
}

bool QGPClient::clientDisconnect()
{
    tcpSocket->disconnectFromHost();
    if (tcpSocket->state() == QTcpSocket::UnconnectedState
            || tcpSocket->waitForDisconnected(1000))
        return true;
    return false;
}

void QGPClient::getData(QList<QByteArray> &data)
{
    dataMutex.lock();
        data.clear();
        dataBuffer.swap(data);
    dataMutex.unlock();
}

void QGPClient::sendCmd(QByteArray cmd)
{
    cmd = cmd + "\r\n";
    tcpSocket->write(cmd, cmd.size());
    tcpSocket->flush();
}
