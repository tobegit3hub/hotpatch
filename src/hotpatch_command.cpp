#include <iostream>
#include <vector>
#include <sstream>
#include <string>
#include <gflags/gflags.h>
#include <glog/logging.h>


#include "hotpatch_command.h"

using namespace std;
using google::INFO;

namespace hotpatch {


HotpatchCommand::HotpatchCommand() {


}

HotpatchCommand::~HotpatchCommand() {

}

void HotpatchCommand::ParseCommand(std::string command) {

    cout << "Parse input command: " << command << endl;

    
    //std::string s = "split on    whitespace   ";
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
            return HandleVarSet(command[2], command[3]);
        } else if (method.compare("get") == 0) {
            return HandleVarGet(command[2]);
        }
    } else if (type.compare("lib") == 0) {
        if (method.compare("list") == 0) {
            return HandleLibList();
        } else if (method.compare("load") == 0) {
            return HandleLibLoad(command[2], command[3]);
        }
    } else if (type.compare("func") == 0) { 
        if (method.compare("list") == 0) {
            return HandleFuncList();
        } else if (method.compare("load") == 0) {
            return HandleFuncLoad(command[2], command[3]);
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
    LOG(INFO) << "Try to set gflags with key: " << key;

    auto result = gflags::SetCommandLineOption(key.c_str(), value.c_str());
    LOG(INFO) << "Set gflags result: " << result;

    return true;
}

bool HotpatchCommand::HandleVarList() {
    return false;
}

bool HotpatchCommand::HandleVarGet(string key) {
    return false;
}

bool HotpatchCommand::HandleVarSet(string key, string value) {
    return false;
}

bool HotpatchCommand::HandleLibList() {
    return false;
}

bool HotpatchCommand::HandleLibLoad(string name, string path) {
    return false;
}

bool HotpatchCommand::HandleFuncList() {
    return false;
}

bool HotpatchCommand::HandleFuncLoad(string lib, string name) {
    return false;
}




} // End of namespace