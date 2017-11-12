#include "judpClient.h"
#include "randomid.h"
#include <QTime>
judpClient::judpClient()
{
    session=new ClientSession();
}

judpClient::~judpClient()
{
    delete session;
}

bool judpClient::bindLocal(QString localIP, int port)
{
    localAddr=localIP;
    localPort=port;
    return true;
}

bool judpClient::bindLocal(int port)
{
    localPort=port;
    return true;
}

bool judpClient::connect(QString host, int port)
{
    bool issucess=true;
    session->host=host;
    session->port=port;
    //
    if(!localAddr.isEmpty())
    {
         session->point=new EndPoint(localAddr,localPort);
         session->point->sessionmodel=client;
    }
    else
    {
          session->point=new EndPoint(localPort);
          session->point->sessionmodel=client;
    }
    session->point->addSession(session);
    session->sender=new Sender(session);//需要接收
    session->point->start();
    //
    QTime time;
    time.start();
    while (session->state<prepare) {
        if(session->state<prepare)
        {
           sendConnet();
        }
        else
        {
            sendConfire();
        }
        //
        if(time.elapsed()>timeOut)
        {
           issucess=false;
           break;
        }
    }
    // 直接连接成功
    //初始化接收发送
    if(issucess)
    {
     session->recvicer=new Recvicer(session);

    }
    return issucess;
}

 void judpClient::sendData(QByteArray data)
{
    session->sendData(data);
}

int judpClient::recvieData(QByteArray &data)
{
    return  session->recvicer->poll(data);
}

void judpClient::setmsTimeOut(int timeout)
{
    timeOut=timeout;
}

void judpClient::close()
{

    session->close();
}

void judpClient::stop()
{
    session->stop();
}

void judpClient::sendConnet()
{
   ConnectPackaget p;
   p.sendTime=QDateTime::currentMSecsSinceEpoch();
   p.isRecvicer=false;
   p.packType=CONNECT;
   p.initNo=RandomStaticID::getSeqNo();
   QByteArray dp=PackagetFactory::createData(&p);
   session->point->write(dp,session->host,session->port);
}

void judpClient::sendConfire()
{
    ConnectPackaget p;
    p.sendTime=QDateTime::currentMSecsSinceEpoch();
    p.isRecvicer=false;
    p.packType=CONFIRE;
    p.initNo=session->initNO;
    QByteArray dp=PackagetFactory::createData(&p);
    session->point->write(dp,session->host,session->port);
}

judpClient *getClient()
{
    return new judpClient();
}
