#include "atomfunctions.h"
#include <iostream>
#include <fstream>
#include <filesystem>
#include <algorithm>
#include "toolsrunner.h"
#include "targetWindow.h"

namespace fs = std::filesystem;

std::vector<std::string> getAtomSubdUrls(const std::string &subdomain, const std::string &workingDir) {
    std::set<std::string> atomSbdUrls;
    for (const auto &entry : fs::recursive_directory_iterator(workingDir)) {
        if (fs::is_regular_file(entry) && entry.path().filename() == "href_links") {
            std::ifstream file(entry.path());
            std::string url;
            while (std::getline(file, url)) {
                if (url.rfind("https://" + subdomain, 0) == 0 || url.rfind("http://" + subdomain, 0) == 0) {
                    atomSbdUrls.insert(url);
                }
            }
        }
    }
    return std::vector<std::string>(atomSbdUrls.begin(), atomSbdUrls.end());
}

std::tuple<bool, std::string, int> atomGuiGetSubdomains(const std::string &projectDirPath, const std::string &toolName) {
    std::string filename;
    if (toolName == "amass") {
        filename = "amassSubdomains.txt";
    } else if (toolName == "sublist3r") {
        filename = "sublisterSubdomains.txt";
    } else if (toolName == "subdomainizer") {
        filename = "subdomainizerSubdomains.txt";
    }

    std::string filepath = projectDirPath + "/" + filename;
    filepath.erase(std::remove(filepath.begin(), filepath.end(), ' '), filepath.end());

    if (!fs::exists(filepath)) {
        return std::make_tuple(false, "", 0);
    } else {
        std::ifstream file(filepath);
        std::string subdomainsStr, line;
        int len_subdomains = 0;
        while (std::getline(file, line)) {
            subdomainsStr += line;
            len_subdomains++;
        }
        if (len_subdomains == 0) {
            return std::make_tuple(false, "", 0);
        } else {
            return std::make_tuple(true, subdomainsStr, len_subdomains);
        }
    }
}

std::string addHttpsScheme(const std::string &link){
    return "none";
}