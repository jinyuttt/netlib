#ifndef UDPSERVER_H
#define UDPSERVER_H

#include "endoint.h"
#include "judpsocket.h"
#include "netlib_global.h"
class NETLIBSHARED_EXPORT judpServer
{
public:
    judpServer(QString host,int port);
    judpServer(int port);
    ~judpServer();
    bool start();
    void stop();
    void close();
    judpSocket accept();
    int  waitTime=30*1000;
private :
    void waitClose();

private:
    EndPoint * point;
};

#endif // UDPSERVER_H
