#pragma once

#include <map>
#include <string>
#include <vector>

namespace util
{

using DbusObjectPath = std::string;
using DbusService = std::string;
using DbusInterface = std::string;
using ObjectTree = std::map<DbusObjectPath,
                            std::map<DbusService, std::vector<DbusInterface>>>;

}
