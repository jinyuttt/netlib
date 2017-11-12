#ifndef UDPSOCKET_H
#define UDPSOCKET_H

#include <QUdpSocket>
#include<QQueue>
#include<QString>
#include <QWaitCondition>
#include<QMutex>
#include <QMutexLocker>
#include<QSemaphore>
class  udpSocket:public QObject
{
public:
    Q_OBJECT
public:
    udpSocket();
    udpSocket(QString& localIP,int& localPort);
    udpSocket(int localPort);
    ~udpSocket();
    int sendData(QByteArray datagram);

    int sendData(QByteArray datagram,QString ip,int port);

    int  recvieData(QByteArray& datagram);

    int  recvieData(QByteArray& datagram,QString &host,int &port);

     void close();
private slots:
    void receive();
private:
    struct  NetData
            {
        QByteArray buffer;
        QString host;
        int port;

    };

    QUdpSocket *uSocket;
    QString remoteHost;
    int remotePort;
    QQueue<NetData> queue;
  //  QWaitCondition bufferIsNotEmpty;
  //  QMutex mutex;
    QSemaphore freeSemaphore;
};

#endif // UDPSOCKET_H
