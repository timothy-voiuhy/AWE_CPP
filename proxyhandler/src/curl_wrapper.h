#include <iostream>
#include <curl/curl.h>
#include <string>
#include <map>

using namespace std;

class Response {
    public:
        long status_code = 0;
        double total_time = 0;
        long content_len;
        string headers;
        string content;

    string get_data(){
        return this->headers+this->content;
    }
};

size_t write_callback(void *contents, size_t size, size_t nmeb, void *userp);
size_t header_callback(void *contents, size_t size, size_t nmeb, void *userp);

class curlConnection{
    private:
        CURL *curl_handle;
    public:
        Response response =  Response();
        struct curl_slist *req_headers;
        char *req_method;
        const char *req_url;
        char *req_data;
        bool has_mtd = false;
        bool has_headers = false;
        bool has_data = false;
        bool follow_redirects = true;
        curlConnection(const char *url);
        curlConnection(char *method, const char *url);
        curlConnection(char *method, const char *url, map<char *, char*> headers);
        curlConnection(char *method, const char *url, char *data, map<char *, char*> headers);
        void appendHeaders(map<char *, char *> &Headers);
        void appendHeader(const char *);
        void curlSetOptions();
        Response request();
        ~curlConnection();
};
