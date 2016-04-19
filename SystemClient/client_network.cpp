#include "client_network.h"
Client_Network::Client_Network(){
    server_socket = 0;
    memset(&server_data, 0, sizeof(server_data));
    memset(&wsa, 0, sizeof(wsa));
    server_ip=IP_ADDRESS;
}
Client_Network::Client_Network(QString ip_a){
    server_socket = 0;
    memset(&server_data, 0, sizeof(server_data));
    memset(&wsa, 0, sizeof(wsa));
    server_ip = ip_a.toStdString();
}

Client_Network::~Client_Network(){
    closesocket(server_socket);
    memset(&server_data, 0, sizeof(server_data));
    memset(&wsa, 0, sizeof(wsa));
}
void Client_Network::Shutdown(){						//same as destructor but used to shutdown network if error occured
    closesocket(server_socket);
    memset(&server_data, 0, sizeof(server_data));
    memset(&wsa, 0, sizeof(wsa));
}

bool Client_Network::Initialise_Lib(){
    if (WSAStartup(MAKEWORD(LIB_VERSION, LIB_VERSION), &wsa) < 0){
      throw NETException{"Failed to load WSA library"};
    }
    return true;
}
bool Client_Network::Create_Socket(){
    server_data.sin_family = AF_INET;
    server_data.sin_port = htons(PORT);
    server_data.sin_addr.s_addr = inet_addr(server_ip.c_str());

    if ((server_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0){
      throw NETException{"Failed to create socket"};
    }
    return true;
}

bool Client_Network::Connect_To_Server(){
    if (connect(server_socket, (struct sockaddr*)&server_data, sizeof(server_data)) < 0){
        throw NETException{"Failed to connect to server"};
    }
    return true;
}



bool Client_Network::Send_Data(const char* send_data){
    if (send_data == NULL){
      throw  NETException{"Bad data"};
    }
    int data_length = strlen(send_data);
    if (send(server_socket, send_data, data_length, NULL) < 0){
       throw NETException{"Failed to send data to server"};
    }
    return true;
}

bool Client_Network::Recv_Data(char* recv_data, int length){
    while (true){
        if (recv(server_socket, recv_data, length, NULL) < 0){
         throw NETException{"Failed to receive data from the server"};
        }
        return true;
    }

}

void Client_Network::Start_Client()
{
    if (!Initialise_Lib()){
        return;
    }
    if (!Create_Socket()){
        return;
    }
}

void Client_Network::Connect()
{
    if (!Connect_To_Server()){
        return;
    }
}

const char *NETException::what()
{
    return excep_name;
}
