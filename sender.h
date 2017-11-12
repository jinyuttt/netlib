#ifndef SENDER_H
#define SENDER_H

#include "netpackaget.h"
#include "session.h"
#include <QThread>
#include <QByteArray>
#include <QQueue>
#include <QMap>
#include <QMutex>
#include <QMutexLocker>
#include <QWaitCondition>
#include "endoint.h"
#include "Util.h"
class Session;
class EndPoint;
class Sender
{
public:
    Sender(Session *session);
    ~Sender();
    void process(Packaget *pack);
    int sendDta(DataPackaget dp);
    void stop();
    bool isEmpty();
    long getLastpackTime();
private:
    void sendReply(Packaget *p);
    void sendReplyConfigre(Packaget *p);
    void doSend();

private:
    Session *session;
    QQueue<DataPackaget> sendQueue;//发送序列
    //
    QMutex mutex;
    QWaitCondition wcondition;

    bool isStop=false;
    //
    int numUsedBytes=0;
    int sleepTime=100;
    //
    long curSeqNo=-1;
    long maxSeqNo=-1;
    bool isrec=false;
public:
    QMap<long,QByteArray> sendBuffer;//缓存
    QMap<long,long> sendTime;//发送时间
    QList<long> lost;//检查重发
};

#endif // SENDER_H
