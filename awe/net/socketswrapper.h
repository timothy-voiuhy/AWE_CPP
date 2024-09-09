#ifndef __HTTPCORE
#define __HTTPCORE

#include<sys/socket.h>
#include<netinet/in.h>
#include<netinet/tcp.h>
#include<string>

using namespace std;

class Socket{
    private:
        int sock_fd;
    public:
        bool is_server;
        string ip;
        int port;
        int netCreateSocket();
        int netAccept();
        int netSetSocketOptions();
        int netBind();
        int netConnect();
        int netSend(const char *data);
        int netListen();
        vector<string> netCreateConnectionSend(const char *data, int r_buf_len);
        /*recieve the data into buffer 
        remember to free the buffer */
        vector<string> netReceive(int sock_fd);
        /*return the ip address of the connected machine*/
        char *netGetConnectedIp();
        Socket(bool server, string ip, int port);
        ~Socket();
};

#endif