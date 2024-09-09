#ifndef CONFIG_H
#define CONFIG_H

#include <string>

// Define constants for different platforms
#ifdef _WIN32  // Windows platform
const std::string HOME_DIR = std::getenv("USERPROFILE");
const std::string CERT_CACHE_DIR = HOME_DIR + "\\AWE\\proxycert\\Certs\\";
const std::string ROOT_CERT_FILE = HOME_DIR + "\\AWE\\proxycert\\CA\\rootCACert.pem";
const std::string PRIVATE_KEY_FILE = HOME_DIR + "\\AWE\\proxycert\\CA\\privatekey.pem";
const std::string DEFAULT_WORKSPACE_DIR = HOME_DIR + "\\AWE\\AtomProjects\\";
const std::string RUNDIR = "D:\\MYAPPLICATIONS\\AWE\\AWE\\crawn\\";
const std::string PROXY_DUMP_DIR = HOME_DIR + "\\AWE\\AtomProjects\\Proxy\\";

#elif defined(__linux__)  // Linux platform
const std::string HOME_DIR = std::getenv("HOME");
const std::string ROOT_CERT_FILE = HOME_DIR + "/AWE/proxycert/CA/rootCACert.pem";
const std::string CERT_CACHE_DIR = HOME_DIR + "/AWE/proxycert/Certs/";
const std::string CERT_KEYS_DIR = HOME_DIR + "/AWE/proxycert/Keys/";
const std::string HOST_CERTS_DIR = HOME_DIR + "/AWE/proxycert/HOST_CERTS";
const std::string PRIVATE_KEY_FILE = HOME_DIR + "/AWE/proxycert/CA/privatekey.pem";
const std::string CERTIFICATE_FILE = HOME_DIR + "/AWE/proxycert/CA/awe_cert.crt";
const std::string DEFAULT_WORKSPACE_DIR = HOME_DIR + "/AWE/AtomProjects/";
const std::string RUNDIR = "/media/kali/data_drive/MYAPPLICATIONS/AWE/AWE/crawn/";
const std::string PROXY_DUMP_DIR = HOME_DIR + "/AWE/AtomProjects/Proxy/";

#else
#error "Unsupported platform"
#endif

#endif  // CONFIG_H
