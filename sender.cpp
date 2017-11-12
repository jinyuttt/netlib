#include "sender.h"
#include <QtConcurrent>
#include "packagetfactory.h"
Sender::Sender(Session *ss)
{
    session=ss;
    QtConcurrent::run(this,&Sender::doSend);
}

Sender::~Sender()
{
    delete session;
    session=NULL;
}

/**
 * @brief Sender::process  处理各类消息
 * @param pack
 */
void Sender::process(Packaget *pack)
{
    if(pack->packType==CONNECT)
    {
        //
        if(session->state<prepare)
        {
              session->state=prepare;
              session->initNO=pack->initNo;
              sendReply(pack);
        }
    }
    else
    {
        if(pack->packType==NAK)
        {
            //丢包
           NAKPackaget *NAK=(NAKPackaget*)pack;
           for(int i=0;i<NAK->listNO.size();i++)
           {
              const long no=  NAK->listNO.at(i);
               QMap<long,QByteArray>::const_iterator it=sendBuffer.find(no);
               while (it !=sendBuffer.end()) {
                  session->point->write(it.value(),session->host,session->port);
             }

           }
          NAK->listNO.clear();
        }
        else if (pack->packType==ACK) {
            NAKPackaget *ACK=(NAKPackaget*)pack;
            for(int i=0;i<ACK->listNO.size();i++)
            {
               const long no= ACK->listNO.at(i);
                int r=sendBuffer.remove(no);
                if(r>0)
                {
                    numUsedBytes--;
                }
               sendTime.remove(no);
               lost.removeAll(no);
            }
           if(ACK->listNO.size()>0)
           {
               mutex.lock();
               wcondition.wakeOne();
               mutex.unlock();
           }
            ACK->listNO.clear();

        }
        else if (pack->packType==REPLY) {
            //发送端
            session->state=prepare;
            ConnectPackaget *con=(ConnectPackaget*)pack;
            session->state=prepare;
            session->initNO=pack->initNo;
            session->destSocketid=pack->socketid;
            session->speed=(QDateTime::currentMSecsSinceEpoch()-con->sendTime)/2;
            sendReplyConfigre(pack);
        }
        else if (pack->packType==CONFIRE) {
           //接收端
            if(session->state<ready)
            {
                session->state=ready;
                //
               sendReplyConfigre(pack);//发送一次，交给recvier定时器发送
               //启动接收
               session->recvicer=new Recvicer(session);
               session->recvicer->start();

            }
        }
        else if (pack->packType==SHUTDOWN) {
             session->recvicer->stop();
             session->point->removeSession(session);
             if(session->point->sessionmodel==client)
             {
                 stop();
                 session->recvicer->stop();
                 session->point->stop();
             }
             else
             {
                   stop();
                 session->recvicer->stop();

             }
        }
    }
    delete pack;
}

/**
 * @brief Sender::sendDta  保存发送数据
 * @param dp
 * @return
 */
int Sender::sendDta(DataPackaget dp)
{
    //session 中分包，这里控制发布
    mutex.lock();
    if(numUsedBytes>=Util::getBufSie())
    {
        wcondition.wait(&mutex);
    }
    sendQueue.enqueue(dp);
    mutex.unlock();
    return 0;
}

void Sender::stop()
{
    isStop=true;
}

/**
 * @brief Sender::isEmpty 数据发送完成
 * @return
 */
bool Sender::isEmpty()
{
   if(sendBuffer.isEmpty()&&sendQueue.isEmpty())
   {
       //发送完成没有缓存
       return true;
   }
   return false;
}

/**
 * @brief Sender::getLastpackTime  最近的时间
 * @return
 */
long Sender::getLastpackTime()
{
    //找最大时间
  long id=   sendTime.lastKey();
  long maxTime=  sendTime.value(id);
  return maxTime;
}

void Sender::sendReply(Packaget *p)
{

    //process 中释放p
  p->packType=REPLY;
  QByteArray dp=PackagetFactory::createData(p);
  session->point->write(dp,session->host,session->port);

}

void Sender::sendReplyConfigre(Packaget *p)
{
    //process 中释放p
    p->packType=REPLYCONFIRE;
    QByteArray dp=PackagetFactory::createData(p);
    session->point->write(dp,session->host,session->port);
}

void Sender::doSend()
{
    while(!isStop)
    {

        if(sendQueue.isEmpty())
        {
            QThread::sleep(sleepTime);
        }
        else
        {

          DataPackaget dp= sendQueue.dequeue();
          QByteArray data=PackagetFactory::createData(&dp);
          session->point->write(data,session->host,session->port);
          session->maxNO=dp.curNo;
          sendBuffer[dp.curNo]=data;
          sendTime[dp.curNo]=QDateTime::currentMSecsSinceEpoch();
          numUsedBytes++;//计算缓存
        }
    }
}
