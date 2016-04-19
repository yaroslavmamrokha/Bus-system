#ifndef MYCLIENTTHREAD_H
#define MYCLIENTTHREAD_H
#include "client_network.h"
static QString recvD;
using std::vector;
static vector<QString> recv_data;
class MyClientThread : public QThread
{
private:
    Client_Network* curr_network;
    QString date;
    vector<QString> income_buffer;
    void GetDataBase();
    QString except;
    bool exc_appeared{false};
public:
    //if exception was triggered return true
    bool isExcept();
    //get current exception name
    QString GetExcept();
    MyClientThread(Client_Network*,QString);
    //get current thread data
   vector<QString> GetThreadData();
    void run();
};

#endif // MYCLIENTTHREAD_H
