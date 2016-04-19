#include "myserverthread.h"
#include<QMessageBox>
MyServerThread::MyServerThread(Server_Network* sn, SQLDbaseService *dbs)
{
    curr_network = sn;
    curr_database = dbs;
}

MyServerThread::MyServerThread(SQLDbaseService * dba)
{
    curr_database = dba;
}


void MyServerThread::startWorkWithcClient()
{
    //thread waits until someone connects to server
    std::unique_lock<std::mutex> lck(mt1);
    cond_var.wait(lck);
    lck.unlock();
    request = Client_Request::WAIT_REQUEST;
    char a[10];
    bool stop = false;
    memset(a,0,10);
    while(!stop){
        //recieve client request and start working with it
        curr_network->Recv_Data(a);
        request = atoi(a);
        switch(request){
        case Client_Request::DB_SEARCH_DATE:
            DateSearchRequest();
            break;
        case Client_Request::DB_RESERVE_SEAT:
            ReserveSeat();
            break;
        case Client_Request::DC_REQUEST:
            curr_network->DisconnetClient();
            stop = true;
            break;
        default:
            curr_network->Send_Data("Bad Request!");
        }
    }
}

void MyServerThread::SetOpType(int ot)
{
    op_type = ot;
}

void MyServerThread::UpDateConnected(QString & name)
{
    connection_name = &name;
}

void MyServerThread::run()
{
    //choose thread function based on operation type
    switch(op_type){
    case 1:
        acceptCon();
        break;
    case 2:
        startWorkWithcClient();
        break;
    }
}

MyServerThread::~MyServerThread()
{

}

void MyServerThread::ReserveSeat()
{
    char seat_buf[15];
    QString id;
    QString seat;
    char id_buf[15];
    memset(id_buf,0,15);
    memset(seat_buf,0,15);
    curr_network->Recv_Data(id_buf,15);
    curr_network->Recv_Data(seat_buf,15);
    id = QString::fromStdString(string{id_buf});
    seat = QString::fromStdString(string{seat_buf});
    vector<QString>id_data;
    //search for recieved id in database and update it seats
    id_data = curr_database->SearchInDatabase("idBuslines",id);
    QStringList lst;
    for(auto x : id_data){
        lst = x.split("#");
    }
    lst.pop_back();
    lst.back().push_back(",");
    lst.back().remove(seat+",");
    string a = lst.back().toStdString();
    if(a.empty()){
        a = "No free seats!";
    }else{
        a.pop_back();
    }
    lst.back() = QString::fromStdString(a);
    curr_database->UpdateDatabase(id,DatabaseColumnNames::PLACES,lst.back());
 }

void MyServerThread::DateSearchRequest()
{
    char date_buf[15];
    memset(date_buf,0,15);
    curr_network->Recv_Data(date_buf,15);
    QString tmp_date = QString::fromStdString(string{date_buf});
    if(date.compare(tmp_date)){
    output_buffer.clear();
    output_buffer.reserve(1000);
    date = tmp_date;
    output_buffer = curr_database->SearchInDatabase("Date",date);
    output_buffer.shrink_to_fit();
    }
    QThread::msleep(20);
    curr_network->Send_Data(to_string(output_buffer.size()).c_str());
    QThread::msleep(20);
    for(auto x: output_buffer){
        curr_network->Send_Data(x.toStdString().c_str());
        QThread::msleep(50);
    }
}

void MyServerThread::acceptCon(){
    static int i = 0;
    while(true){
       QString tmp = QString::fromStdString(curr_network->StartAcception());
       //lock to avoid data races;
       mt2.lock();
      *connection_name = tmp;
       mt2.unlock();
       msleep(100);
      {
      copy_net = *curr_network;
      //notify 2 thread to start working with client
     cond_var.notify_one();
      }
    }
}
