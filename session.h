#ifndef SESSION_H
#define SESSION_H


#include "sender.h"
class EndPoint;
class Recvicer;
class Sender;
 class Session
{
public:
    Session();
    ~Session();
    void sendData(QByteArray dp);
     void close();
     void stop();
     int  recviceData(QByteArray &data,QString &host,int &port);
 private:
     void delayClose();
 public:
    long curNO=-1;
    long maxNO=-1;
    long initNO=-1;
    long socketid=-1;
    long destSocketid=-1;
    int speed;
    Recvicer *recvicer;
    Sender * sender;
    ConnectSate state=start;
    EndPoint *point;
    QString  host;
    int port;
 private:
   int size=1475;
   int delayTime=10;//spee倍数；否则为10s;
};

#endif // SESSION_H
