#include <iostream>
#include <vector>
#include <sstream>
#include <string>
#include <dlfcn.h>
#include <memory>
#include <cassert>
#include <gflags/gflags.h>
#include <glog/logging.h>

#include "hotpatch_command.h"

using namespace std;
using google::INFO;

namespace hotpatch {


HotpatchCommand::HotpatchCommand(std::map<std::string, void*> &registered_variables, std::map<std::string, void*> &registered_libraries, std::map<std::string, void*> &registered_dl_handlers, std::map<std::string, void*> &registered_functions): _registered_variables(registered_variables), _registered_libraries(registered_libraries), _registered_dl_handlers(registered_dl_handlers), _registered_functions(registered_functions) {

}

HotpatchCommand::~HotpatchCommand() {

}

void HotpatchCommand::ParseCommand(std::string command) {
    LOG(INFO) << "Parse input command: " << command;

    // Split string with space to vector
    std::vector<std::string> split_keys;
    std::istringstream iss(command);
    for(std::string s; iss >> s; ) {
        split_keys.push_back(s);
    }

    // TODO: Check command format
    HandleCommand(split_keys);
}

bool HotpatchCommand::HandleCommand(std::vector<std::string> command) {
    
    auto type = command[0];
    auto method = command[1];

    if (type.compare("gflags") == 0) {
        if (method.compare("list") == 0) {
            return HandleGflagsList();
        } else if (method.compare("set") == 0) {
            return HandleGflagsSet(command[2], command[3]);
        } else if (method.compare("get") == 0) {
            return HandleGflagsGet(command[2]);
        }
    } else if (type.compare("var") == 0) {
        if (method.compare("list") == 0) {
            return HandleVarList();
        } else if (method.compare("set") == 0) {
            return HandleVarSet(command[2], command[3], command[4]);
        } else if (method.compare("get") == 0) {
            return HandleVarGet(command[2], command[3]);
        }
    } else if (type.compare("lib") == 0) {
        if (method.compare("list") == 0) {
            return HandleLibList();
        } else if (method.compare("load") == 0) {
            return HandleLibLoad(command[2], command[3]);
        } else if (method.compare("unload") == 0) {
            return HandleLibUnload(command[2]);
        }
    } else if (type.compare("func") == 0) { 
        if (method.compare("upgrade") == 0) {
            return HandleFuncUpgrade(command[2], command[3]);
        } else if (method.compare("rollback") == 0) {
            return HandleFuncRollback(command[2]);
        }
    } 

    return false;
}

bool HotpatchCommand::HandleGflagsList() {
    LOG(INFO) << "Try to list all gflags";

    std::vector<gflags::CommandLineFlagInfo> flags;
    gflags::GetAllFlags(&flags);
    std::vector<gflags::CommandLineFlagInfo>::iterator i;
    for (i = flags.begin(); i != flags.end(); ++i) {
        LOG(INFO) << "Flag name: " << i->name << ", value: " << i->current_value;
    }            

    return true;
}

bool HotpatchCommand::HandleGflagsGet(string key) {
    LOG(INFO) << "Try to get gflags with key: " << key;

    gflags::CommandLineFlagInfo flag;
    auto result = gflags::GetCommandLineFlagInfo(key.c_str(), &flag);

    LOG(INFO) << "Flag name: " << flag.name << ", value: " << flag.current_value;

    return result;
}

bool HotpatchCommand::HandleGflagsSet(string key, string value) {
    LOG(INFO) << "Try to set gflags with key: " << key << ", value: " << value;

    auto result = gflags::SetCommandLineOption(key.c_str(), value.c_str());
    LOG(INFO) << "Set gflags result: " << result;

    return true;
}

bool HotpatchCommand::HandleVarList() {
    LOG(INFO) << "Try to list all variables";

    for(std::map<std::string, void*>::iterator it = _registered_variables.begin(); it != _registered_variables.end(); ++it) {
        LOG(INFO) << "Get variable: " << it->first << ", pointer: " << it->second;
    }
    
    return true;
}


bool HotpatchCommand::HandleVarGet(string type, string key) {
    LOG(INFO) << "Try to get variable: " << key << ", type: "<< type;

    // TODO: Support most primitive types
    if (type.compare("string") == 0) {
        LOG(INFO) << *reinterpret_cast<std::string*>(_registered_variables[key]);
    } else if (type.compare("int") == 0) {
        LOG(INFO) << *reinterpret_cast<int*>(_registered_variables[key]);
    } else if (type.compare("double") == 0) {
        LOG(INFO) << *reinterpret_cast<double*>(_registered_variables[key]);
    } 

    return true;
}

bool HotpatchCommand::HandleVarSet(string type, string key, string value) {

    if (type.compare("string") == 0) {
        std::string &v = *reinterpret_cast<std::string*>(_registered_variables[key]);
        v = value;
    } else if (type.compare("int") == 0) {
        int &v = *reinterpret_cast<int*>(_registered_variables[key]);
        v = std::stoi(value);
    } else if (type.compare("doble") == 0) {
        double &v = *reinterpret_cast<double*>(_registered_variables[key]);
        v = std::stod(value);
    } 

    return true;
}

bool HotpatchCommand::HandleLibList() {
    LOG(INFO) << "Try to list all dynamic libraries";

    // List all the registered libraries which are recorded in memory
    for(std::map<std::string, void*>::iterator it = _registered_dl_handlers.begin(); it != _registered_dl_handlers.end(); ++it) {
        LOG(INFO) << "Get library: " << it->first << ", pointer: " << it->second;
    }

    return true;
}

bool HotpatchCommand::HandleLibLoad(string name, string path) {
    LOG(INFO) << "Try to load dynamic library: " << name << ", path: " << path;

    // Load the dynamic library and register in memory
    _registered_dl_handlers[name] = dlopen(path.c_str(), RTLD_LAZY);
    if (!_registered_dl_handlers[name] ) {
        std::cerr << "Cannot open library: " << dlerror() << '\n';
        return false;
    }

    return true;
}

bool HotpatchCommand::HandleLibUnload(string name) {
    LOG(INFO) << "Try to unload dynamic library: " << name;

    // Close the dl handler
    if (_registered_dl_handlers[name] != NULL) {
        dlclose(_registered_dl_handlers[name]);
    }

    // Remove the key in map
    _registered_dl_handlers.erase(name);

    return true;
}


bool HotpatchCommand::HandleFuncUpgrade(string lib_name, string func_name) {
    LOG(INFO) << "Try to upgrade function from dynamic library: " << lib_name << ", function name: " << func_name;

    if (_registered_libraries.find(lib_name) == _registered_libraries.end() ) {
        auto dl_handler = _registered_dl_handlers[lib_name];

        // TODO: Handle if method name is different from registered name
        auto origin_func = _registered_functions[func_name];

        auto load_func = dlsym(dl_handler, func_name.c_str());

        // TODO: Release the old one when register the one with the same name
        subhook_t func_hook = subhook_new((void *)origin_func, (void *)load_func, SUBHOOK_64BIT_OFFSET);

        subhook_install(func_hook);

        _registered_func_subhook[func_name] = func_hook;

    }

    return true;
}

bool HotpatchCommand::HandleFuncRollback(string func_name) {
    LOG(INFO) << "Try to rollback function: " << func_name;

    // TODO: Check if it is registered or not
    subhook_t func_subhook = _registered_func_subhook[func_name];

    subhook_remove(func_subhook);
    subhook_free(func_subhook);

    return true;
}


} // End of namespace