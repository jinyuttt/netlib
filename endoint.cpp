#include "endoint.h"
#include "packagetfactory.h"
#include "serversession.h"
#include "randomsocketid.h"
#include "session.h"
EndPoint::~EndPoint()
{
    mapdest.clear();
    sendQueue.clear();
    qDeleteAll(dataQueue);
    dataQueue.clear();
    QMap<long,Session*>::iterator it;
    for(it=mapSession->begin();it!=mapSession->end();++it)
      {
          delete it.value();
          it.value()=NULL;
      }
      mapSession->clear();
      delete mapSession;
      mapSession=NULL;
     delete    usocket;
    usocket=NULL;
}
EndPoint::EndPoint()
{
    usocket=new udpSocket();
    mapSession=new QMap<long,Session*>();
}

EndPoint::EndPoint(QString ip, int port)
{
    usocket=new udpSocket(ip,port);
    mapSession=new QMap<long,Session*>();
}

EndPoint::EndPoint(int port)
{
    usocket=new udpSocket(port);
    mapSession=new QMap<long,Session*>();
}

void EndPoint::start()
{
   QtConcurrent::run(this,&EndPoint::doRecvice);

}

void EndPoint::sendData(QByteArray data)
{
   sendQueue.enqueue(data);
}

void EndPoint::write(QByteArray data, QString host, int port)
{
    usocket->sendData(data,host,port);
}

/**
 * @brief EndPoint::addSession 保存
 * @param session
 *
 */
void EndPoint::addSession(Session *session)
{
     mapSession->insert(session->socketid,session);
}

/**
 * @brief EndPoint::removeSession 移除
 * @param session
 */
void EndPoint::removeSession(Session *session)
{
    mapSession->remove(session->socketid);
}

/**
 * @brief EndPoint::removeSession 移除
 * @param id
 */
void EndPoint::removeSession(long id)
{
     mapSession->remove(id);
}

/**
 * @brief EndPoint::stop  停止通讯
 */
void EndPoint::stop()
{
    isStop=true;
    usocket->close();
}

/**
 * @brief EndPoint::addSocket 添加有数据的session
 * @param session
 */
void EndPoint::addSocket(Session *session)
{
    recQSemaphore.release();
    dataQueue.push_back(session);
    //移除其余无效session;
    removeOther(session->socketid,session->destSocketid);
}

/**
 * @brief EndPoint::accept 返回发送端的通讯对象
 * @return
 */
judpSocket EndPoint::accept()
{
    recQSemaphore.acquire();
    Session * session=  dataQueue.dequeue();
    judpSocket socket(session);
    return socket;

}

/**
 * @brief EndPoint::addOther  添加同一个目的socket的session(相当于按照目的id分组）
 * @param session
 */
void EndPoint::addOther(Session *session)
{
  QList<long> ids=  mapdest.value(session->destSocketid);
  ids.push_back(session->socketid);
}

/**
 * @brief EndPoint::removeOther 移除无数据socket
 * @param id
 * @param destid
 */
void EndPoint::removeOther(long id, long destid)
{
    QList<long> ids=  mapdest.value(destid);
    for(int i=0;i<ids.size();i++)
    {
        if(ids.at(i)!=id)
        {
          mapSession->remove(ids.at(i));
        }
    }
    ids.clear();
    mapdest.remove(destid);
}

void EndPoint::doRecvice()
{
    QByteArray data;
    while(!isStop)
    {
        QString host;
        int port;
       int r= usocket->recvieData(data,host,port);
       if(r>0)
       {
           //
         Packaget *p=  PackagetFactory::createPack(data);
         Session *session;
         if(p->packType==CONNECT)
         {
             //连接

             if(sessionmodel==client)
                 {
                  continue;//不正确的数据
             }
            session=  mapSession->value(p->destSocketid,NULL);
           if(session==NULL)
           {
             session=new ServerSession();
             session->point=this;
             session->initNO=p->initNo;
             session->socketid=RandomSocketID::getID();
             session->destSocketid=p->socketid;
             addSession(session);
             addOther(session);
             session->host=host;
             session->port=port;
             //启动senser;recvier在接收到确认后启动
             session->sender=new Sender(session);
           }
         }
         //
        if(p->isRecvicer)
        {
            session->recvicer->process(p);
        }
        else
        {
            session->sender->process(p);
        }
       }

    }
    //结束了
    usocket->close();
}
