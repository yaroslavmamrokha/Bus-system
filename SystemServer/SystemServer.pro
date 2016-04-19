#-------------------------------------------------
#
# Project created by QtCreator 2016-03-12T22:25:03
#
#-------------------------------------------------

QT       += core gui
LIBS += -lws2_32
CONFIG += c++11
QT += sql
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SystemServer
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    sqldbaseservice.cpp \
    servernetwork.cpp \
    myserverthread.cpp

HEADERS  += mainwindow.h \
    sqldbaseservice.h \
    servernetwork.h \
    samelibs.h \
    myserverthread.h

FORMS    += mainwindow.ui
