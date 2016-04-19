#ifndef SERVERNETWORK_H
#define SERVERNETWORK_H
#include "samelibs.h"
using std::exception;
#define IP_ADDRESS "178.212.99.204" //default ip address
#define PORT 5973 //default port
#define  DEFAULT_LENGHT 10
#define LIB_VERSION 2
#pragma comment (lib, "ws2_32.lib")


class NETException:public exception{
private:
    QString Name;
public:
    NETException(QString nm):Name{nm}{}
    const char* what();
};


class Server_Network{
private:
    WSADATA wsa;
    SOCKET server_socket, client_socket;
    struct sockaddr_in server_data, client_data;
    int client_data_size;
    //initialise winsock lib (For windows only)
    bool Initialise_Lib();
    //create server socket
    bool Create_Socket();
    // bind current socket to machine
    bool Establish_Server();
    //accept connetion from client
    bool Accept_Connection();


public:
    //make copy of current network
    void Copy(Server_Network&);
    Server_Network();
    //start clients acception, if failed throw exception, else return connected ip
    std::string StartAcception();
    //send data to client
    bool Send_Data(const char* send_data);
    //recieve data from client, default value DEFAULT_LENGHT macros
    bool Recv_Data(char* recv_data, int length = DEFAULT_LENGHT);
    //sutdown all network
    void Shutdown();
    //get current connected IP
    QString currentIP();
    //disconnect client
    void DisconnetClient();
    //set server on current machine
    void Start_Server();
    ~Server_Network();
};

#endif // SERVERNETWORK_H
