#ifndef RANDOMSOCKETID_H
#define RANDOMSOCKETID_H

#include <QMutex>
#include <QMutexLocker>
class RandomSocketID
{
public:
    RandomSocketID();

   static long getID()
    {
         QMutexLocker locker(&mutex);
          id++;
          return id;
    }
private :
   static  long id;
   static QMutex mutex;
};

#endif // RANDOMSOCKETID_H
