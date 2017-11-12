#ifndef ENDOINT_H
#define ENDOINT_H

#include<QtConcurrent/QtConcurrent>
#include "udpsocket.h"
#include "session.h"
#include "comtype.h"
#include "recvicer.h"
#include "judpsocket.h"
class Session;
class judpSocket;
class EndPoint
{
public:
   ~EndPoint();
    EndPoint();
    EndPoint(QString ip,int port);
    EndPoint(int port);
      void start();
      //void doSend();
      void sendData(QByteArray data);
      void write(QByteArray data,QString host,int port);
      void addSession(Session *session);
      void removeSession(Session *session);//接收关闭消息时清除
      void stop();
      void addSocket(Session *session);
      judpSocket accept();
      SessionModel sessionmodel=client;
 private :
  void doRecvice();
  void removeSession(long id);
  void addOther(Session *session);
  void removeOther(long id,long destid);
private:
  udpSocket *usocket;
  QReadWriteLock lockrw;
  QMutex mutex;
  QMap<long,Session*> *mapSession;
  QQueue<QByteArray> sendQueue;
  QThreadPool   threadpool;  //
  QMap<long,QList<long>>  mapdest;
  QQueue<Session*> dataQueue;
  int sleepTime=100;
   bool isStop=false;
  QSemaphore recQSemaphore;

};



#endif // ENDOINT_H
