#ifndef JUDPSRVTHREAD_H
#define JUDPSRVTHREAD_H

#include <QByteArray>
#include <QThread>
#include "netlib_global.h"
#include "judpsocket.h"
#include "judpserver.h"
#include <QMap>
class NETLIBSHARED_EXPORT judpSrvThread:public QThread
{

public:
    Q_OBJECT
 public:
     bool isSocket=true;//限制是使用socketReady信号还是resultThread信号
     explicit judpSrvThread(QObject *parent = 0);
          explicit judpSrvThread(QString *host, int *port);
     void run();

     /**
      * @brief stop
      * 立即关闭
      */
     void stop();

     /**
      * @brief close
      * 等数据发送完成后关闭
      */
     void close();
 signals:
  void socketReady(judpSocket socket);//发送socket信号，直接获取socket操作
  void resultThread(long socketid,QByteArray data,QString host,int port,QString msg);//只发送数据，内部使用socket;
 public slots:
  bool  SocketCallData(long socketid,QByteArray data);//反发数据
  void  SocketClose(long socketid);
private :
  judpServer *userver;
  QMap<long,judpSocket> mapSocket;
  bool isStop=false;
private:
  void  MultThreadRecvice(judpSocket usocket);
};
extern "C" {
     NETLIBSHARED_EXPORT judpSrvThread* getSrvThread(QString host,int port); //获取类SharedLibrary对象
 }
#endif // JUDPSRVTHREAD_H
