#include "myclientthread.h"

void MyClientThread::GetDataBase()
{
    char size_buf[15];
    char line[100];
    memset(line,0,100);
    memset(size_buf,0,15);
    try{
        //send request with data to server; if failed triggers exception
    curr_network->Send_Data("0");
    curr_network->Send_Data(date.toStdString().c_str());
    //recieve answer
    curr_network->Recv_Data(size_buf,15);
    }catch(NETException& ex){
        exc_appeared = true;
        except = QString::fromStdString(std::string{ex.what()});
    }
    //receive database and store to income_buffer buffer
    for(int i = 0; i < atoi(size_buf); i++){
        try{

        curr_network->Recv_Data(line,100);
        }catch(NETException& ex){
            exc_appeared = true;
            except = QString::fromStdString(std::string{ex.what()});
        }
        income_buffer.push_back(QString::fromStdString(std::string{line}));
        memset(line,0,100);
    }


}

bool MyClientThread::isExcept()
{
    return exc_appeared;
}

QString MyClientThread::GetExcept()
{
    return except;
}

MyClientThread::MyClientThread(Client_Network *cn, QString dt)
{
    curr_network = cn;
    date = dt;
}

vector<QString> MyClientThread::GetThreadData()
{
    return income_buffer;
}

void MyClientThread::run()
{
    GetDataBase();
}
