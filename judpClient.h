#ifndef UDPCLIENT_H
#define UDPCLIENT_H

#include "session.h"
#include "clientsession.h"
#include "endoint.h"
#include "netlib_global.h"
#include <QObject>
class NETLIBSHARED_EXPORT judpClient:public QObject
{
public:
    Q_OBJECT
public:
  explicit  judpClient();
    ~judpClient();
    ClientSession *session;
    EndPoint point;
    bool bindLocal(QString localIP,int port);
    bool bindLocal(int port);
    bool connect(QString host,int port);
    void sendData(QByteArray data);
    int recvieData(QByteArray &data);
    void setmsTimeOut(int timeout);
    void close();
    void stop();
private:
    void sendConnet();
    void sendConfire();
    int timeOut=60*1000;//ms
    QString localAddr="";
    int localPort=0;

};
extern "C" {
     NETLIBSHARED_EXPORT judpClient* getClient(); //获取类SharedLibrary对象
 }
#endif // UDPCLIENT_H
