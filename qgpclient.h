#ifndef QGPCLIENT_H
#define QGPCLIENT_H

#include <QObject>
#include <QThread>
#include <QTcpSocket>
#include <QMutex>

class QGPClient : public QObject
{
    Q_OBJECT
private:
    unsigned int _ip_port;
    QString _ip_address;

    bool _rx_status;
    bool _stop_thread;

    int _rx_buffer_size;
    QList<QByteArray> _in_buffer;
    QList<QByteArray> _out_buffer;

    QTcpSocket *tcpSocket;
    QThread thread;

private slots:
    void readData();

public:
    QGPClient(QObject * parent = NULL);

    void setAddress (QString address) {_ip_address = address;} // set server IP address
    void setPort (unsigned int port) {_ip_port = port;} // set server IP port

    bool clientConnect();
    bool clientDisconnect();

    void sendCmd(QByteArray cmd);
    void getData(QList<QByteArray> &data);

    int getState(){return tcpSocket->state();}
};

#endif // QGPCLIENT_H
