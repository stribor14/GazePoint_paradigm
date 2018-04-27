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

    unsigned int ipPort;
    QString ipAddress;

    QList<QByteArray> buffer;
    int bufferSize;
    QMutex bufferMutex;

private slots:
    void receiveData();

public:
    QGPClient(QObject * parent = NULL);
    ~QGPClient();

    void setAddress (QString address) {ipAddress = address;} // set server IP address
    void setPort (unsigned int port) {ipPort = port;} // set server IP port

    bool clientConnect();
    bool clientDisconnect();

    void sendCmd(QByteArray cmd);

    void getMsgBuffer(QList<QByteArray> &data);
    QByteArray getLastMsg(); // WARNING: clearing the buffer content

    QTcpSocket::SocketState getState(){return tcpSocket->state();}
    QTcpSocket::SocketError getLastError(){return tcpSocket->error();}

signals: // forwarded from QTcpSocket
    void hostFound();
    void connected();
    void disconnected();
    void stateChanged(QTcpSocket::SocketState);
    void error(QTcpSocket::SocketError);
};

#endif // QGPCLIENT_H
