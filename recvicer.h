#ifndef RECVICER_H
#define RECVICER_H

#include "netpackaget.h"
#include <QQueue>
#include <QDateTime>
#include "session.h"
#include "packagetfactory.h"
#include "recvicebuffer.h"
#include "Util.h"
class Session;
class Recvicer
{
public:
    Recvicer(Session *curSession);
    ~Recvicer();
   void process(Packaget *pack);
   void start();
   int  poll(QByteArray &data);
    void stop();
 private:
   void sendACK();
   void sendNAK();
   void doRecvice();
   void sendShutdown();
   void addLost(long no,long last);
   void sendLost();
private:
   QQueue<DataPackaget> *recBuffer;
   long curTime=0;
   long keepTime=0;
   long ackTime=0;
   long nakTime=0;
   long lostTime=0;
   long lastTime=0;
   bool isStop=false;
   bool isRec=false;
   long lastPackNo=0;
   Session *session;
   RecviceBuffer * buffer;
   QList<long> lost;
   QList<long> recSeqNo;//接受的序列号

};

#endif // RECVICER_H
