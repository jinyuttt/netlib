#ifndef JUDPSOCKET_H
#define JUDPSOCKET_H

#include <QByteArray>
#include "session.h"
#include "netlib_global.h"
class Session;
class NETLIBSHARED_EXPORT judpSocket
{
public:
    judpSocket(Session *ss);
    ~judpSocket();
    void sendData(QByteArray data);
    int recviceData(QByteArray &data,QString &host,int &port);
    void close();
     bool isClose();
public:
    long socketid=-1;

private:
    Session *session;
    bool iscolse=false;
};

#endif // JUDPSOCKET_H
