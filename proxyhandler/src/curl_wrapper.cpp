#include <iostream>
#include <string>
#include <curl/curl.h>
#include <cstring>
#include <syslog.h>
#include "curl_wrapper.h"
#include <map>

using namespace std;

size_t write_callback(void *contents, size_t size, size_t nitems, void *userp)
{
    size_t total_size = size*nitems;
    string *response = static_cast<string*>(userp);
    response->append(static_cast<char*>(contents), total_size);
    return total_size;
}

size_t header_callback(void *contents, size_t size, size_t nitems, void *userp){
    size_t total_size = size*nitems;
    string *headers = static_cast<string*>(userp);
    headers->append(static_cast<char*>(contents), total_size);
    return total_size;
}

curlConnection::curlConnection(const char *url){
    this->curl_handle = curl_easy_init();
    this->req_url = url;
}

curlConnection::curlConnection(char *method, const char *url){
    this->curl_handle = curl_easy_init();
    this->has_mtd = true;
    this->req_method = method;
    this->req_url = url;
}

curlConnection::curlConnection(char *method, const char *url, map<char *, char*> headers){
    this->curl_handle = curl_easy_init();
    this->has_mtd = true;
    this->has_headers = true;
    this->req_method = method;
    this->req_url = url;
    this->appendHeaders(headers);
}

curlConnection::curlConnection(char *method, const char *url, char *data, map<char *, char*> headers){
    this->curl_handle = curl_easy_init();
    this->has_mtd = true;
    this->has_headers = true;
    this->has_data = true;
    this->req_url = url;
    this->req_method = method;
    this->req_data = data;
    this->appendHeaders(headers);
}

void curlConnection::curlSetOptions(){
    curl_easy_setopt(this->curl_handle, CURLOPT_URL, this->req_url);
    if(this->has_mtd == true){
        curl_easy_setopt(this->curl_handle, CURLOPT_CUSTOMREQUEST, this->req_method);
    }
    if(this->has_headers == true){
        curl_easy_setopt(this->curl_handle, CURLOPT_HTTPHEADER, this->req_headers);
    }
    if(this->has_data == true){
        curl_easy_setopt(this->curl_handle, CURLOPT_POSTFIELDS, this->req_data);
    }
    if(this->follow_redirects){
        curl_easy_setopt(this->curl_handle, CURLOPT_FOLLOWLOCATION, 1L);
    }else{
        curl_easy_setopt(this->curl_handle, CURLOPT_FOLLOWLOCATION, 0L);
    }
    curl_easy_setopt(this->curl_handle, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(this->curl_handle, CURLOPT_HEADERFUNCTION, header_callback);
    curl_easy_setopt(this->curl_handle, CURLOPT_WRITEDATA, &this->response.content);
    curl_easy_setopt(this->curl_handle, CURLOPT_HEADERDATA, &this->response.headers);
}

void curlConnection::appendHeader(const char *header){
    if(this->has_headers==true){
        this->req_headers = curl_slist_append(this->req_headers, header);
    }
}

void curlConnection::appendHeaders(map<char*, char*> &Headers){
    if(this->has_headers){
        for(const auto& pair:Headers){
            string delimeter = ": ";
            string header = string(pair.first)+pair.second+delimeter;
            this->appendHeader(header.c_str());
        }
    }
}

Response curlConnection::request(){
    this->curlSetOptions();
    CURLcode res = curl_easy_perform(this->curl_handle);
    if(res == CURLE_OK){
        curl_easy_getinfo(this->curl_handle, CURLINFO_RESPONSE_CODE, &this->response.status_code );
        curl_easy_getinfo(this->curl_handle, CURLINFO_CONTENT_LENGTH_DOWNLOAD_T, &this->response.content_len);
        curl_easy_getinfo(this->curl_handle, CURLINFO_TOTAL_TIME, &this->response.total_time);
    }else{
        cout << "curl_easy_perform() failed with error: " << curl_easy_strerror(res) << endl;
    }
    return this->response;
}

curlConnection::~curlConnection(){
    curl_easy_cleanup(this->curl_handle);
}