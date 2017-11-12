#include "recvicer.h"
#include "Util.h"
#include <QThread>
#include "endoint.h"
Recvicer::Recvicer(Session *curSession)
{
    recBuffer=new  QQueue<DataPackaget>();
    session=curSession;
    buffer=new RecviceBuffer(curSession->initNO,2048);
    lastTime=QDateTime::currentMSecsSinceEpoch();
}

Recvicer::~Recvicer()
{
    delete session;
    session=NULL;
    delete buffer;
    buffer=NULL;
}

void Recvicer::process(Packaget *pack)
{
    if(isStop)
    {
        return;
    }
    //一定是数据包或者keepalive
    if(pack->packType==DATA)
    {
        if(!isRec)
        {
            //第一次
            session->point->addSocket(session);
        }
        DataPackaget *p=(DataPackaget*)pack;
        AppData dp={p->curNo,p->data};
        if(buffer->offer(&dp))
        {
            //
           if(p->curNo>lastPackNo)
           {
               if(lastPackNo+1<p->curNo)
               {
                   //中间没有
                   addLost(p->curNo,lastPackNo);
               }
               lastPackNo=p->curNo;
           }
           recSeqNo.push_back(p->curNo);
        }
        isRec=true;
    }
    else if (pack->packType==KEPPALIVE) {
        keepTime=QDateTime::currentMSecsSinceEpoch();

    }
    //无论哪种数据都算存活接收
    lastTime=QDateTime::currentMSecsSinceEpoch();
    delete pack;
}

void Recvicer::start()
{
    QtConcurrent::run(this,&Recvicer::doRecvice);
}

int Recvicer::poll(QByteArray &data)
{
  return  buffer->poll(data);
}

void Recvicer::sendACK()
{
  if(!isRec)
  {
      ConnectPackaget con;
      con.destSocketid=session->destSocketid;
      con.socketid=session->socketid;
      con.initNo=session->initNO;
      con.packType=REPLYCONFIRE;
      QByteArray data=PackagetFactory::createData(&con);
      session->point->write(data,session->host,session->port);
  }
  else
  {
      ACKPackaget ack;
      ack.destSocketid=session->destSocketid;
      ack.initNo=session->initNO;
      ack.socketid=session->socketid;
      ack.isRecvicer=false;
      ack.speed=session->speed;
      ack.num=recSeqNo.size();
      ack.listNO.append(recSeqNo);
      QByteArray data=PackagetFactory::createData(&ack);
      recSeqNo.clear();
      session->point->write(data,session->host,session->port);
  }
}

void Recvicer::sendNAK()
{
    NAKPackaget nak;
    nak.destSocketid=session->destSocketid;
    nak.initNo=session->initNO;
    nak.socketid=session->socketid;
    nak.isRecvicer=false;
    nak.speed=session->speed;
    nak.num=lost.size();
    nak.listNO.append(lost);
    QByteArray data=PackagetFactory::createData(&nak);
    lost.clear();
    session->point->write(data,session->host,session->port);

}

void Recvicer::doRecvice()
{
    while(!isStop)
    {

     if(curTime<ackTime)
     {
          ackTime=ackTime+Util::getACK();
          sendACK();
     }
     if(curTime<nakTime)
     {
        nakTime=nakTime+Util::getNAK();
        sendNAK();
     }
     if(curTime<lostTime)
     {
         lostTime=lostTime+Util::getLost();//检测sendBuffer
         sendLost();
     }
     if(curTime<keepTime)
     {
         keepTime=keepTime+Util::getKeepAlive();
         long sp=QDateTime::currentMSecsSinceEpoch()-lastTime;
         if(sp>Util::getKeepHeart())
         {
             //超时；
            sendShutdown();
            session->state=close;
            stop();
         }
     }
     //
     QThread::sleep(50);
     curTime+=50;
    }
}

void Recvicer::sendShutdown()
{
    ShutDownPackaget shut;
    shut.destSocketid=session->destSocketid;
    shut.socketid=session->socketid;
    shut.packType=SHUTDOWN;
    QByteArray tmp=PackagetFactory::createData(&shut);
    session->point->write(tmp,session->host,session->port);
}

void Recvicer::stop()
{
    isStop=true;
    sendShutdown();
}

void Recvicer::addLost(long no, long last)
{
   //
    while (last<no) {
        if(!lost.contains(last))
        {
            lost.push_back(last);
            last++;
        }

    }
}

/**
 * @brief Recvicer::sendLost  发送端数据重发
 */
void Recvicer::sendLost()
{
 QList<long> list=  session->sender->sendBuffer.keys();
 for(int i=0;i<list.size();i++)
 {
     long id= list[i];
     long time= session->sender->sendTime.value(id);
     if(QDateTime::currentMSecsSinceEpoch()-time>Util::getACK()+session->speed)
     {
         //sender重发
        if(!session->sender->lost.contains(id))
        {
            session->sender->lost.push_back(id);
        }
     }
 }
}
