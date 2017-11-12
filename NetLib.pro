#-------------------------------------------------
#
# Project created by QtCreator 2017-10-23T01:02:55
#
#-------------------------------------------------

QT       -= gui

TARGET = NetLib
TEMPLATE = lib

DEFINES += NETLIB_LIBRARY
DESTDIR+=../plugins
# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += netlib.cpp \
    udpsocket.cpp \
    endoint.cpp \
    recvicer.cpp \
    sender.cpp \
    session.cpp \
    clientsession.cpp \
    serversession.cpp \
    recvicethread.cpp \
    packagetfactory.cpp \
    randomsocketid.cpp \
    recvicebuffer.cpp \
    staticinit.cpp \
    judpsocket.cpp \
    judpnet.cpp \
    judpsrvthread.cpp \
    judpserver.cpp \
    judpclient.cpp

HEADERS += netlib.h\
        netlib_global.h \
    udpsocket.h \
    endoint.h \
    recvicer.h \
    sender.h \
    comtype.h \
    session.h \
    clientsession.h \
    serversession.h \
    recvicethread.h \
    netpackaget.h \
    packagetfactory.h \
    randomid.h \
    randomsocketid.h \
    recvicebuffer.h \
    judpsocket.h \
    judpnet.h \
    judpsrvthread.h \
    judpserver.h \
    Util.h \
    judpClient.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
QT += network
 QT += concurrent
