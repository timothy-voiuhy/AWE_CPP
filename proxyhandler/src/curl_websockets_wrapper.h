#include <curl/curl.h>
#include <iostream>


using namespace std;

class curlWebSocketConnection
{
    private:
        CURL *curl_handle;
    public:
        char *url;
        curlWebSocketConnection(char *url);
        int ping();
        int recvPong();
        CURLcode recv();
        void websocket();
        void websocketClose();
        void setOptions();
        void requestConnection();
        ~curlWebSocketConnection();
};
