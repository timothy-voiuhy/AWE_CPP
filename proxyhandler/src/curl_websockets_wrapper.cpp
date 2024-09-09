#include <curl/curl.h>
#include <cstring>
#include <unistd.h>
#include "curl_websockets_wrapper.h"

curlWebSocketConnection::curlWebSocketConnection(char *url){
    this->curl_handle = curl_easy_init();
    this->url = url;
}

curlWebSocketConnection::~curlWebSocketConnection(){
    curl_easy_cleanup(this->curl_handle);
}

int curlWebSocketConnection::ping(){
    size_t sent;
    const char *payload = "ping";
    CURLcode result =curl_ws_send(this->curl_handle, payload, strlen(payload), &sent, 0,CURLWS_PING);
    return (int)result;
}

int curlWebSocketConnection::recvPong(){
    size_t rlen;
    const struct curl_ws_frame *meta;
    char buffer[256];
    char *expected_payload = "pong";
    CURLcode result = curl_ws_recv(this->curl_handle, buffer, sizeof(buffer), &rlen, &meta);
    if (!result)
    {
        if (meta->flags & CURLWS_PONG)
        {
        int same = 0;
        fprintf(stderr, "ws: got PONG back\n");
        if (rlen == strlen(expected_payload))
        {
            if (!memcmp(expected_payload, buffer, rlen))
            {
            fprintf(stderr, "ws: got the same payload back\n");
            same = 1;
            }
        }
        if (!same)
            fprintf(stderr, "ws: did NOT get the same payload back\n");
        }
        else
        {
        fprintf(stderr, "recv_pong: got %u bytes rflags %x\n", (int)rlen,
                meta->flags);
        }
    }
    fprintf(stderr, "ws: curl_ws_recv returned %u, received %u\n",
            (unsigned int)result, (unsigned int)rlen);
    return (int)result;
}

CURLcode curlWebSocketConnection::recv(){
    size_t rlen;
    const struct curl_ws_frame *meta;
    char buffer[256];
    return curl_ws_recv(this->curl_handle, buffer, sizeof(buffer), &rlen, &meta);
}

void curlWebSocketConnection::websocketClose(){
    size_t sent;
    (void)curl_ws_send(this->curl_handle, "", 0, &sent, 0, CURLWS_CLOSE);
}

void curlWebSocketConnection::setOptions(){
    curl_easy_setopt(this->curl_handle, CURLOPT_URL, &this->url);
    curl_easy_setopt(this->curl_handle, CURLOPT_CONNECT_ONLY, 2L);
}

void curlWebSocketConnection::websocket()
{
    int i = 0;
    do
    {
        this->recv();
        if (this->ping()){return;}
        if (this->recvPong()){return;}
        sleep(2);
    } while (i++ < 10);
    this->websocketClose();
}

void curlWebSocketConnection::requestConnection(){
    this->setOptions();
    CURLcode res = curl_easy_perform(this->curl_handle);
    if(res == CURLE_OK){
        this->websocket();
    }
}