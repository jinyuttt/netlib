#include "udpsocket.h"

udpSocket::udpSocket()
{
    uSocket=new QUdpSocket();
    connect(uSocket,SIGNAL(readyRead()), this, SLOT(receive()));
}

udpSocket::udpSocket(QString& localIP, int& localPort)
{
    uSocket=new QUdpSocket();
     uSocket->bind(QHostAddress(localIP), localPort);
     connect(uSocket, SIGNAL(readyRead()), this, SLOT(receive()));
}

udpSocket::udpSocket(int localPort)
{
     uSocket=new QUdpSocket();
    uSocket->bind(localPort);
    connect(uSocket, SIGNAL(readyRead()), this, SLOT(receive()));
}

udpSocket::~udpSocket()
{
  delete uSocket;
}

int udpSocket::sendData(QByteArray datagram)
{
  return uSocket->writeDatagram(datagram,QHostAddress(remoteHost),remotePort);
}

int udpSocket::sendData(QByteArray datagram, QString ip, int port)
{
 return   uSocket->writeDatagram(datagram,QHostAddress(ip),port);
}
/**
 * @brief udpSocket::recvieData
 * @param datagram 数据
 * @return
 */
int udpSocket::recvieData(QByteArray& datagram)
{
   // mutex.lock();
  //  if(queue.isEmpty())
  //  {
        freeSemaphore.acquire();
  //    //  bufferIsNotEmpty.wait(&mutex);
  //   }
    NetData data= queue.dequeue();
    datagram.clear();
    datagram.resize(data.buffer.size());
    datagram.append(data.buffer);
 //   mutex.unlock();
    return data.buffer.size();
}
/**
 * @brief udpSocket::recvieData
 * @param datagram 接收数据数组
 * @param host 接收来源IP
 * @param port 接收来源端口
 * @return 0 正常 -1 错误
 */
int udpSocket::recvieData(QByteArray& datagram, QString &host, int &port)
{
   //  mutex.lock();
     freeSemaphore.acquire();
     try
     {
     if(!queue.empty())
     {
       NetData data= queue.dequeue();
       datagram.clear();
       datagram.push_back(data.buffer);
       host=data.host;
       port=data.port;
   //  mutex.unlock();
      return data.buffer.size();
     }
     else
     {
         return 0;
     }
     }
     catch(...)
     {
         return 0;
     }

}

void udpSocket::close()
{
    uSocket->close();
}
/**
 * @brief udpSocket::receive
 * 接受网络数据
 */
void udpSocket::receive()
{
    QByteArray buffer;
    while(uSocket->hasPendingDatagrams())
    {
        buffer.resize(uSocket->pendingDatagramSize());
        QHostAddress sender;//发送者的地址
        quint16 senderPort;//发送者的端口号
        uSocket->readDatagram(buffer.data(),(quint64)buffer.size(),&sender,&senderPort);//读取数据报
        NetData data;
        data.buffer.clear();
        data.buffer.push_back(buffer);
        data.host=sender.toString();
        data.port=senderPort;
      //  mutex.lock();
        queue.enqueue(data);
      //  bufferIsNotEmpty.wakeOne();
        freeSemaphore.release();
      //  mutex.unlock();

    }

}
