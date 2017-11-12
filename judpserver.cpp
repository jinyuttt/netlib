#include "judpserver.h"


judpServer::judpServer(QString host, int port)
{
     point=new EndPoint(host,port);
     point->sessionmodel=server;
}

judpServer::judpServer(int port)
{
    point=new EndPoint(port);
    point->sessionmodel=server;
}

judpServer::~judpServer()
{
   delete point;
}

bool judpServer::start()
{
    point->start();
    return true;
}

void judpServer::stop()
{
    point->stop();
    delete point;
    point=NULL;
}

void judpServer::close()
{

}

judpSocket judpServer::accept()
{
    return  point->accept();
}

void judpServer::waitClose()
{

}
