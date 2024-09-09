#include <sys/socket.h>
#include <arpa/inet.h>
#include <cstring>
#include <vector>

#include "socketswrapper.h"

using namespace std;

int Socket::netCreateSocket(){
    this->sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(this->sock_fd == -1){
        perror("socket creation failure");
        return EXIT_FAILURE;
    }
}

int Socket::netSetSocketOptions(){
    int reuse = 1;
    if(setsockopt(this->sock_fd,SOL_SOCKET,SO_REUSEADDR, &reuse, sizeof(reuse))== -1){
        return EXIT_FAILURE;
    };
    return EXIT_SUCCESS;
}

int Socket::netBind(){
    struct sockaddr_in addr;
    addr.sin_addr.s_addr = inet_addr(this->ip.c_str());
    addr.sin_port = htons(port);
    addr.sin_family = AF_INET;
    if(bind(sock_fd,(struct sockaddr *)&addr, sizeof(addr)) == -1){
        perror("Failed to bind socket to port");
        exit(EXIT_FAILURE);
    }
    return EXIT_SUCCESS;   
}

int Socket::netConnect(){
    struct sockaddr_in conn_addr;
    conn_addr.sin_addr.s_addr = inet_addr(this->ip.c_str());
    conn_addr.sin_family = AF_INET;
    conn_addr.sin_port = htons(this->port);
    if(connect(sock_fd,(struct sockaddr *)&conn_addr,sizeof(conn_addr)) == -1){
        perror("connection failure");
        return EXIT_FAILURE;}
    return EXIT_SUCCESS;
}

char *Socket::netGetConnectedIp(){
    struct sockaddr_in connected_ip;
    socklen_t connected_ip_len = sizeof(connected_ip);
    if(getpeername(this->sock_fd,(struct sockaddr *)&connected_ip,&connected_ip_len)!=0){
        perror("get peer name");
    }
    return inet_ntoa(connected_ip.sin_addr);
}

Socket::Socket(bool server, string __ip, int __port)
{
    if(server == true){
        this->is_server = true;
        this->ip = __ip;
        this->port = __port;
}
    }

int Socket::netListen(){
    if(listen(this->sock_fd, 5) == -1){
        perror("listen() failed");
        return EXIT_FAILURE;
    }else{
        return EXIT_SUCCESS;
    };
}

int Socket::netAccept(){
    struct sockaddr_in connected_ip;
    socklen_t len_connected_ip = sizeof(connected_ip);
    int new_sockfd = accept(this->sock_fd, (struct sockaddr *)&connected_ip,&len_connected_ip);
    if(new_sockfd == -1){
        perror("failed to accept connection");
        return EXIT_FAILURE;
    }else{
        printf("%s","new connection accepted\n");
        return new_sockfd;
    }
}

int Socket::netSend(const char *data){
    if(send(sock_fd,data,strlen(data),0) == -1){
        perror("Send Data failure");
        return EXIT_FAILURE;
    };
}

vector<string> Socket::netCreateConnectionSend(const char *data, int r_buf_len){
    // r_buf_len is the expected return buffer length
    this->sock_fd = netCreateSocket(); 
    netConnect();
    netSend(data);
    return netReceive(r_buf_len);
}

vector<string> Socket::netReceive(int buffer_len){
    char *buffer = (char *)malloc(buffer_len);
    ssize_t recievedBytes = recv(sock_fd, buffer, 4096, 0);
    if(recievedBytes < 0){
        perror("Recieving Failure");
    }
    vector<string> response_;
    response_.push_back(string(buffer));
    response_.push_back(to_string(recievedBytes));
    return response_;
}