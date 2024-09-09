#ifndef ATOM_TOOLS_H
#define ATOM_TOOLS_H

#include <string>
#include <set>
#include <vector>
#include <tuple>

#include "toolsrunner.h"

std::vector<std::string> getAtomSubdUrls(const std::string &subdomain, const std::string &workingDir);

std::tuple<bool, std::string, int> atomGuiGetSubdomains(const std::string &projectDirPath, const std::string &toolName);

std::vector<std::string> atomGuiGetUrls(const std::string &subdomain, const std::string &workingDir, void *parent = nullptr,
                                        void *top_parent = nullptr, void *mainWindow = nullptr);
std::string addHttpsScheme(const std::string &link);

#endif // ATOM_TOOLS_H
