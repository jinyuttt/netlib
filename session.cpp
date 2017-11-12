#include "session.h"
#include "randomsocketid.h"
Session::Session()
{
    curNO=initNO;
    socketid=RandomSocketID::getID();
}

Session::~Session()
{
    delete sender;
    delete recvicer;
}

void Session::sendData(QByteArray dp)
{
    int dpsize=dp.size();
    int num=dpsize/size+dpsize%size==0?0:1;
    int len=0;
    int index=0;
    for(int i=0;i<num;i++)
    {
        len=dpsize-index>size?size:dpsize-index;
         QByteArray data=DataByte::subPart(dp,index,len);
         index+=len;
         DataPackaget p;
         p.curNo=++curNO;
         p.data=data;
         p.destSocketid=destSocketid;
         p.initNo=initNO;
         p.isRecvicer=true;
         p.packType=DATA;
         p.socketid=socketid;
         p.speed=speed;
         sender->sendDta(p);
    }
}

void Session::close()
{
   //session 改变
    if(sender->isEmpty())
    {
        stop();
    }
    else
    {
        QtConcurrent::run(this,&Session::delayClose);
    }
}

void Session::stop()
{
    //立即关闭
    recvicer->stop();
    sender->stop();
    if(point->sessionmodel==client)
    {
        point->stop();
        delete point;
        point=NULL;
    }
}

int Session::recviceData(QByteArray &data, QString &host, int &port)
{

    host=this->host;
    port=this->port;
    return  recvicer->poll(data);
}

void Session::delayClose()
{
    long waitTime=delayTime*speed>0?delayTime*speed:10*1000;
    while(QDateTime::currentMSecsSinceEpoch()-sender->getLastpackTime()<waitTime)
    {
        stop();
    }
}
