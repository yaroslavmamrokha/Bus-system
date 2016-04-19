#ifndef SAMELIBS_H
#define SAMELIBS_H
#include<QTableWidget>
#include<QThread>
#include<winsock2.h>
#include<QString>
#include<exception>
#include<QtCore/QCoreApplication>
#include<QDebug>
#include<QtSql>
#include<condition_variable>
#include<mutex>
static std::mutex mt1;
static std::mutex mt2;
static std::condition_variable cond_var;
#endif // SAMELIBS_H
