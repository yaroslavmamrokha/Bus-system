#include "servernetwork.h"
Server_Network::Server_Network(){
    server_socket = 0;
    client_socket = 0;
    client_data_size = sizeof(client_data);
    memset(&server_data, 0, sizeof(server_data));
    memset(&client_data, 0, sizeof(client_data));
    memset(&wsa, 0, sizeof(wsa));
  }

Server_Network::~Server_Network(){
    Shutdown();
}

bool Server_Network::Initialise_Lib(){
    if (WSAStartup(MAKEWORD(LIB_VERSION, LIB_VERSION), &wsa) < 0){
        return false;
    }
    return true;
}

bool Server_Network::Create_Socket(){

    server_data.sin_addr.s_addr = INADDR_ANY;
    server_data.sin_family = AF_INET;
    server_data.sin_port = htons(PORT);

    if ((server_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0){
        return false;
    }
    return true;
}


void Server_Network::Shutdown(){

    closesocket(server_socket);
    memset(&server_data, 0, sizeof(server_data));
    memset(&wsa, 0, sizeof(wsa));
}

QString Server_Network::currentIP()
{
    return QString::fromStdString(std::string{inet_ntoa(client_data.sin_addr)});
}

void Server_Network::DisconnetClient()
{
     closesocket(client_socket);
     memset(&client_data, 0, sizeof(client_data));
}

void Server_Network::Start_Server()
{
    if (!Initialise_Lib()){
       throw NETException{"Failed to load winsock library, closing program..."};
    }
    if (!Create_Socket()){
        throw NETException{"Failed to create socket, closing program..."};
    }
    if (!Establish_Server()){
        closesocket(server_socket);
       throw NETException{"Failed to Create a server, try again later..." };
    }
}



bool Server_Network::Establish_Server(){
    if (bind(server_socket, (struct sockaddr*)&server_data, sizeof(server_data)) < 0){
        return false;
    }
    if (listen(server_socket, SOMAXCONN) < 0){
        return false;
    }
    return true;
}

bool Server_Network::Send_Data(const char* send_data){
    if (send_data == NULL){
        throw NETException{"Bad data passed to send...."};
    }
    if (send(client_socket, send_data, strlen(send_data), NULL) < 0){
         throw NETException{"Failed to send data to client...."};
    }
    return true;
}

bool Server_Network::Recv_Data(char* recv_data, int length){
    while (true){
        if (recv(client_socket, recv_data, length, NULL) < 0){
            throw NETException{"Failed to receive data from client...."};
        }
        return true;
    }
}

bool Server_Network::Accept_Connection(){
    if ((client_socket = accept(server_socket, (struct sockaddr*)&client_data, &client_data_size)) < 0){
        throw NETException{"Failed to accept incomming connection...."};
    }
    return true;
}

void Server_Network::Copy(Server_Network & b)
{
    *this =b;
}

std::string Server_Network::StartAcception(){

     if(!Accept_Connection()){
        closesocket(server_socket);
        throw NETException{"Failed to Accept a server, try again later..."};
    }
     return currentIP().toStdString();
}


const char *NETException::what()
{
    return Name.toStdString().c_str();
}
