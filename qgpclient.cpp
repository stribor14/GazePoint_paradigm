#include "qgpclient.h"

void QGPClient::readData()
{
    int delimiter_index;
    while(!_stop_thread && tcpSocket->state() == QTcpSocket::ConnectedState){
        QByteArray data = tcpSocket->readAll();
        delimiter_index = data.indexOf("\r\n", 0);
        while (data.size())
        {
            QByteArray tmp = data.mid(0, delimiter_index);

            // save record at head of queue (FIFO)
            _in_buffer.append(tmp);

            // remove records longer than queue size (so we don't run out of memory)
            while (_in_buffer.size() > _rx_buffer_size)
            {
                _in_buffer.removeFirst();
            }

            data.remove(0, delimiter_index+2);
            delimiter_index = data.indexOf("\r\n", 0);
        }

        while(_out_buffer.size()){
            tcpSocket->write(_out_buffer.takeFirst());
        }
    }
}

QGPClient::QGPClient(QObject * parent) : QObject(parent),
    tcpSocket(new QTcpSocket()),
    thread(new QThread(this))
{
    _ip_port = 4242;
    _ip_address = "127.0.0.1";

    _rx_buffer_size = 60*60*3;
    _rx_status = false;

    moveToThread(&thread);
    connect(&thread, &QThread::started, this, &QGPClient::readData);
}

bool QGPClient::clientConnect()
{
    tcpSocket->connectToHost(_ip_address, _ip_port);
    if (!tcpSocket->waitForConnected(1000)) return false;

    _stop_thread = false;
    thread.start();
    return true;
}

bool QGPClient::clientDisconnect()
{
    _stop_thread = true;
    thread.wait();
    tcpSocket->disconnectFromHost();
    if (tcpSocket->state() == QTcpSocket::UnconnectedState
            || tcpSocket->waitForDisconnected(1000))
        return true;
    return false;
}

void QGPClient::getData(QList<QByteArray> &data)
{
    data.clear();
    _out_buffer.swap(data);
}

void QGPClient::sendCmd(QByteArray cmd)
{
    cmd = cmd + "\r\n";
    _out_buffer.append(cmd);
}
