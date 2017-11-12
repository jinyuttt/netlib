#ifndef PACKAGETFACTORY_H
#define PACKAGETFACTORY_H

#include "netpackaget.h"
#include <QByteArray>
class PackagetFactory
{
public:
    PackagetFactory();
    static Packaget* createPack(QByteArray data)
    {
           int lenlong=sizeof(long);
           int lenint=sizeof(int);
           int index=0;
           Packaget* p;
           long curNo=DataByte::convertToInt(data,0,lenlong);
           long initNo=DataByte::convertToLong(data,lenlong,lenlong);
           PackagetType packType=(PackagetType)DataByte::convertToInt(data,2*lenlong,lenint);
           long socketid=DataByte::convertToInt(data,2*lenlong+lenint,lenlong);
           long destSocketid=DataByte::convertToInt(data,3*lenlong+lenint,lenlong);
           int speed=DataByte::convertToInt(data,4*lenlong+lenint,lenint);
            bool isRecvicer=(bool)DataByte::subPart(data,4*lenlong+2*lenint,1).at(0);
            index=4*lenlong+2*lenint+1;
            switch (packType) {
            case CONNECT:
            case  REPLY:
            case REPLYCONFIRE:
            case  CONFIRE:
            {
                ConnectPackaget * con=new ConnectPackaget();
                con->sendTime=DataByte::convertToLong(data,index,lenlong);
                p=con;
                break;
            }
            case ACK:
            {
                ACKPackaget *ack=new ACKPackaget();
                ack->num=DataByte::convertToInt(data,index,lenint);
                ack->listNO=CopyData(data,index+lenint,ack->num);
                ack->waitTime=DataByte::convertToInt(data,index+index+lenint+ack->num*lenlong,lenint);
                p=ack;
                break;
            }
                case NAK:
            {
                NAKPackaget *nak=new NAKPackaget() ;
                nak->num=DataByte::convertToInt(data,index,lenint);
                nak->listNO=CopyData(data,index+lenint,nak->num);
                p=nak;
                 break;
            }
               case SHUTDOWN:
            {
                ShutDownPackaget *shut=new ShutDownPackaget();
                p=shut;
                 break;
            }
            case KEPPALIVE:
            {
                KeepAlivePackaget *keep=new KeepAlivePackaget();
                p=keep;
                 break;
            }
            case DATA:
            {
                DataPackaget *dp=new DataPackaget();
                dp->data=data.right(data.size()-index);
                dp->curNo=curNo;
                break;
            }
             default:
                break;
            }
  p->destSocketid=destSocketid;
  p->initNo=initNo;
  p->isRecvicer=isRecvicer;
  p->packType=packType;
  p->socketid=socketid;
  p->speed=speed;
  return p;
    }
    static QByteArray createData(Packaget *pack)
    {
          QByteArray dp;
           dp.push_back(DataByte::convertLongToByte(0));
           dp.push_back(DataByte::convertLongToByte(pack->initNo));
           dp.push_back(DataByte::convertIntToByte(pack->packType));
           dp.push_back(DataByte::convertLongToByte(pack->socketid));
           dp.push_back(DataByte::convertLongToByte(pack->destSocketid));
           dp.push_back(DataByte::convertIntToByte(pack->speed));
           dp.push_back(pack->isRecvicer);



                //
           switch (pack->packType) {
                case CONNECT:
                {
                   ConnectPackaget *con=(ConnectPackaget*)pack;
                   dp.push_back(DataByte::convertLongToByte(con->sendTime));
                   break;
                }
                case ACK:
                {
                    ACKPackaget *ack=(ACKPackaget*)pack;
                    dp.push_back(DataByte::convertIntToByte(ack->num));
                    dp.push_back(CopyList(ack->listNO));
                    dp.push_back(DataByte::convertLongToByte(ack->waitTime));
                    break;
                }

                case NAK:
                {
                    NAKPackaget *nak=(NAKPackaget*)pack;
                    dp.push_back(DataByte::convertIntToByte(nak->num));
                    dp.push_back(CopyList(nak->listNO));
                    break;
                }
                case DATA:
                {
                    int lenlong=sizeof(long);
                    DataPackaget *d=(DataPackaget *)pack;
                    dp.push_back(d->data);
                    dp= dp.remove(0,lenlong);
                    dp= dp.insert(0,DataByte::convertLongToByte(d->curNo));
                    break;
                }
                default:
                    break;
                }
   return dp;
    }
    static QList<long> CopyData(QByteArray data,int index,int num)
    {
          int lenlong=sizeof(long);
          int len=num*lenlong;//
          QList<long> list;
          QByteArray part=DataByte::subPart(data,index,len);
          int start=0;
          for(int i=0;i<num;i++)
          {
              long id=DataByte::convertToLong(part,start,lenlong);
              list.push_back(id);
              start+=i*lenlong;
          }
        return list;
    }
    static QByteArray CopyList(QList<long> list)
    {
          QByteArray dp;
          for(int i=0;i<list.size();i++)
          {
              dp.push_back(DataByte::convertLongToByte(list.at(i)));
          }
          return dp;
    }
};

#endif // PACKAGETFACTORY_H
