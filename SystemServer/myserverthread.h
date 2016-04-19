#ifndef MYSERVERTHREAD_H
#define MYSERVERTHREAD_H
#include "servernetwork.h"
#include "sqldbaseservice.h"
static QString RecvD;
static Server_Network copy_net;

enum Client_Request{
    DB_SEARCH_DATE,
    DB_RESERVE_SEAT,
    DC_REQUEST,
    WAIT_REQUEST
};

class MyServerThread : public QThread
{
private:
    int op_type;
    Server_Network* curr_network; //current network pointer
    QString* connection_name; //connected ip
    SQLDbaseService* curr_database; //current database pointer
    QString date;
    int request;
    vector<QString>output_buffer; //output buffer
protected:
    //reserves seat recieved from client
    void ReserveSeat();
    //search in database for date recieved from client
    void DateSearchRequest();
    //accepts connection from clients
    void acceptCon();
    //start request-answer logic with client
    void startWorkWithcClient();
public:
    MyServerThread(Server_Network*, SQLDbaseService*);
    MyServerThread(SQLDbaseService*);
    //sets current operation type
    void SetOpType(int);
    void UpDateConnected(QString&);
    //based on operation type: Start accepting connection or start working with client
    void run();
    ~MyServerThread();
};

#endif // MYSERVERTHREAD_H
