#include "judpsrvthread.h"



judpSrvThread::judpSrvThread(QObject *parent)
{

}

judpSrvThread::judpSrvThread(QString *host, int *port)
{
     userver=new judpServer(*host,*port);
}

void judpSrvThread::run()
{
    userver->start();
    while (!isStop) {
       judpSocket usocket=  userver->accept();

        if(isSocket)
        {
          emit socketReady(usocket);//先发送，然后再启动线程;
        }
        else
        {
             mapSocket.insert(usocket.socketid,usocket);
           QtConcurrent::run(this,&judpSrvThread::MultThreadRecvice,usocket);
        }

    }
}

void judpSrvThread::stop()
{
    isStop=true;
   userver->stop();
}

void judpSrvThread::close()
{
    isStop=true;
    userver->stop();
}

bool judpSrvThread::SocketCallData(long socketid, QByteArray data)
{
    QMap<long,judpSocket>::iterator it= mapSocket.find(socketid);
    if(it!=mapSocket.end()&&it.key()==socketid)
    {
        it.value().sendData(data);
        return true;
    }
    return false;
}


void judpSrvThread::SocketClose(long socketid)
{
 QMap<long,judpSocket>::iterator it=   mapSocket.find(socketid);
 if(it!=mapSocket.end()&&it.key()==socketid)
 {
     it.value().close();
     mapSocket.remove(socketid);
 }

}

void judpSrvThread::MultThreadRecvice(judpSocket usocket)
{
    QByteArray recData;
    recData.resize(1475);
    QString host;
    int port;
    while (!usocket.isClose()) {
         int r= usocket.recviceData(recData,host,port);
         if(r>0)
         {
            emit resultThread(usocket.socketid,recData,host,port,"");
         }

    }
}

judpSrvThread *getSrvThread(QString host, int port)
{
    return new judpSrvThread(&host,&port);
}
