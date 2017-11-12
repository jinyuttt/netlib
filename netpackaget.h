#ifndef NETPACKAGET_H
#define NETPACKAGET_H

#include <QList>
#include "comtype.h"
struct Packaget
{
    long initNo;
    PackagetType packType;
    long socketid;
    long destSocketid;
    int speed;
    bool isRecvicer;
};
struct ConnectPackaget:Packaget
{

    //回复，确认，回复确认相同
    long sendTime;
};
struct NAKPackaget:Packaget
{
    int  num;
    QList<long> listNO;
};
struct ACKPackaget:Packaget
{
    //接收完成
    int waitTime;
    int num;
    QList<long> listNO;
};
struct KeepAlivePackaget:Packaget
{

};
struct ShutDownPackaget:Packaget
{

};
struct DataPackaget:Packaget
{
     long curNo;
    QByteArray data;
    int speed;
};
 struct ResultAll:ConnectPackaget,NAKPackaget,ACKPackaget
 {

 };

struct AppData
{
    long curNo;
    QByteArray data;
};
 class DataByte
{
 public:
    static QByteArray subPart(QByteArray data,int start,int len)
    {
        QByteArray part;
        part.resize(len);
       // for(int i=start;i<start+len;i++)
      //  {
             memcpy(part.data(),data.data()+start,len); //指针移动,写入多个数据
           // part.push_back(data.at(i));
       // }
        return part;
    }

   static int convertToInt(QByteArray data,int start,int len)
    {
      //  bool ok;
        QByteArray part=subPart(data,start,len);
        int intdata=0;
             //   part.toInt(&ok,10);
        int *tmp=&intdata;
        memcpy(tmp,part.data(),len); //指针移动,写入多个数据
      //  intdata=*tmp;
        return intdata;

    }
    static long convertToLong(QByteArray data,int start,int len)
    {
      //  bool ok;
        QByteArray part=subPart(data,start,len);
        long longdata=0;
          //      part.toLong(&ok,10);
        long *tmp=&longdata;
        memcpy(tmp,part.data(),len); //指针移动,写入多个数据
      //  longdata=*tmp;
        return longdata;

    }
    static QByteArray convertIntToByte(int value)
    {
        QByteArray data;
        int len=sizeof(int);
        data.resize(len);
        memcpy(data.data(),&value,len);
        return data;
    }
    static QByteArray convertLongToByte(long value)
    {
        QByteArray data;
        int len=sizeof(long);
        data.resize(len);
        memcpy(data.data(),&value,len);
        return data;
    }

};

#endif // NETPACKAGET_H
