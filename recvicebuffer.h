#ifndef RECVICEBUFFER_H
#define RECVICEBUFFER_H

#include "netpackaget.h"
#include <QMutex>
#include <QMap>
typedef AppData* pAppData;
class RecviceBuffer
{
public:
    RecviceBuffer(long initSeqNO, int size);
    ~RecviceBuffer();
    bool offer(AppData* data);
    int poll(QByteArray &data);
    void copyData(QByteArray data);
private:
    void updateblocak();
private:
    pAppData *buffer;
    long initNO=-1;
    int capacity=0;
    QMap<long,QString>mapSeqNO;
    int dataNum=0;
    QMutex mutex;
    int readIndex=0;
    AppData *appData;
    int index=0;
};

#endif // RECVICEBUFFER_H
