#include "judpnet.h"
#include "judpsocket.h"
#include "judpClient.h"
#include "netlib_global.h"
judpNet::judpNet()
{

}

judpServer *getServer(QString host, int port)
{
   return new judpServer(host,port);
}
