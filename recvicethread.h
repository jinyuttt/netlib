#ifndef RECVICETHREAD_H
#define RECVICETHREAD_H

#include "udpsocket.h"
#include <QByteArray>
#include <QThread>
#include "netlib_global.h"
class NETLIBSHARED_EXPORT RecviceThread : public QThread
{
public:
    Q_OBJECT
 public:
     bool stop=false ;
     explicit RecviceThread(QObject *parent = 0);
          explicit RecviceThread(QString *host, int *port);
     void run();
 signals:
  void socketReady(udpSocket* socket);
  void resultThread(QByteArray data,QString host,int port,QString msg);
 public slots:
  bool  SocketCallData(QByteArray data,QString host,int port);
private :
  udpSocket *usocket;
};
extern "C" {
     NETLIBSHARED_EXPORT RecviceThread* getRecThread(QString host,int port); //获取类SharedLibrary对象
 }
#endif // RECVICETHREAD_H
