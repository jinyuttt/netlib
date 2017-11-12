#ifndef JUDPNET_H
#define JUDPNET_H

#include "netlib_global.h"
#include "judpsocket.h"
#include "judpserver.h"
class judpNet
{
public:
    judpNet();
};
extern "C" {
     NETLIBSHARED_EXPORT judpServer *getServer(QString host,int port); //获取类SharedLibrary对象
 }
#endif // JUDPNET_H
