#include<curl/curl.h>
#include "curl_wrapper.h"
#include <string>

using namespace std;

int main(int argc, char const *argv[])
{
    curl_global_init(CURL_GLOBAL_DEFAULT);
    string url = "https://hackerone.com/";
    curlConnection connection  = curlConnection(url.c_str());
    connection.follow_redirects = false;
    Response resp = connection.request();
    cout << "server response: " << resp.get_data() << endl;
    curl_global_cleanup();
    
    return 0;
}
