#include "recvicethread.h"



RecviceThread::RecviceThread(QString* host, int* port)
{

    int localPort=*port;
    if(host->isNull()||host->isEmpty())
    {

       usocket=new udpSocket(localPort);
    }
    else
    {
           QString localIP=*host;
           usocket=new udpSocket(localIP,localPort);
    }
}

bool RecviceThread::SocketCallData(QByteArray data, QString host, int port)
{
    usocket->sendData(data,host,port);
    return true;
}

void RecviceThread::run()
{
    QByteArray data;
    QString host;
    int port;
     while(!stop)
     {
         usocket->recvieData(data,host,port);
         emit resultThread(data,host,port,"123");

     }
}


RecviceThread *getRecThread(QString host, int port)
{
     return new RecviceThread(&host,&port);
}
