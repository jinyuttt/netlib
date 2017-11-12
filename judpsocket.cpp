#include "judpsocket.h"

judpSocket::judpSocket(Session *ss)
{
    session=ss;
    socketid=ss->socketid;
}

judpSocket::~judpSocket()
{
    delete session;
    session=NULL;
}

void judpSocket::sendData(QByteArray data)
{
      session->sendData(data);
}

int judpSocket::recviceData(QByteArray &data, QString &host, int &port)
{
    return  session->recviceData(data,host,port);
}

void judpSocket::close()
{
    iscolse=true;
    session->close();
}

bool judpSocket::isClose()
{
    return iscolse;
}
