#ifndef COMTYPE_H
#define COMTYPE_H


enum ConnectSate
{
    start,
    prepare,
    ready,
    close
};
enum PackagetType
{
    CONNECT,
    ACK,
    NAK,
    SHUTDOWN,
    KEPPALIVE,
    REPLY,
    REPLYCONFIRE,
    CONFIRE,
    DATA
};
enum SessionModel
{
    client,
    server
};
#endif // COMTYPE_H
