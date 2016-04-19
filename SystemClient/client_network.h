#ifndef CLIENT_NETWORK_H
#define CLIENT_NETWORK_H
#include "samelibs.h"
#define IP_ADDRESS "178.212.99.204"
#define PORT 5973
#define  DEFAULT_LENGHT 20
#define LIB_NAME "ws2_32.lib"
#define LIB_VERSION 2
using std::exception;
using std::string;
#pragma comment(lib,LIB_NAME)
class NETException:public exception{
private:
    const char* excep_name;
public:
    NETException(const char* nm):excep_name{nm}{}
    const char *what();
};


class Client_Network{
private:
    WSADATA wsa;
    SOCKET server_socket;
    struct sockaddr_in server_data;
    string server_ip; //server ip
    bool Initialise_Lib();
    bool Create_Socket();
    bool Connect_To_Server();
public:
    //close connection to server
    void Shutdown();
    Client_Network();
    Client_Network(QString);
    //send data to  server
    bool Send_Data(const char* send_data);
    //receive data from server
    bool Recv_Data(char* recv_data, int length = DEFAULT_LENGHT);
    //initialise all data requiered for connection
    void Start_Client();
    //connect to server
    void Connect();
    ~Client_Network();
};

#endif // CLIENT_NETWORK_H
