#-------------------------------------------------
#
# Project created by QtCreator 2016-02-20T19:37:46
#
#-------------------------------------------------

QT       += core gui
QT += sql
LIBS += -lws2_32
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
CONFIG += c++11

TARGET = SystemClient
TEMPLATE = app
DEFINES += NOMINMAX
SOURCES += main.cpp\
        mainwindow.cpp \
    payment.cpp \
    client_network.cpp \
    myclientthread.cpp

HEADERS  += mainwindow.h \
    payment.h \
    client_network.h \
    samelibs.h \
    myclientthread.h

FORMS    += mainwindow.ui \
    payment.ui
