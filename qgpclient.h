#ifndef QGPCLIENT_H
#define QGPCLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QMutex>

class QGPClient : public QObject
{
    Q_OBJECT
private:
    QTcpSocket *tcpSocket;
    QMutex dataMutex;
    unsigned int ipPort;
    QString ipAddress;
    int bufferSize;
    QList<QByteArray> dataBuffer;

private slots:
    void readData();

public:
    QGPClient(QObject * parent = NULL);

    void setAddress (QString address) {ipAddress = address;} // set server IP address
    void setPort (unsigned int port) {ipPort = port;} // set server IP port

    bool clientConnect();
    bool clientDisconnect();

    void sendCmd(QByteArray cmd);
    void getData(QList<QByteArray> &data);

    int getState(){return tcpSocket->state();}
};

#endif // QGPCLIENT_H
