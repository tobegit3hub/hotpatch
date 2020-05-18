#ifndef _H_HOTPATCH_COMMAND
#define _H_HOTPATCH_COMMAND

#include <string>
#include <map>
#include <vector>
#include <subhook.h>

using namespace std;

namespace hotpatch {

class HotpatchCommand {

private:
    std::map<std::string, void*> &_registered_variables;
    std::map<std::string, void*> &_registered_libraries;
    std::map<std::string, void*> &_registered_dl_handlers;

    std::map<std::string, void*> &_registered_functions;
    std::map<std::string, subhook::Hook> _registered_subhook;


public:

    HotpatchCommand(std::map<std::string, void*>& registered_variables, std::map<std::string, void*>& registered_libraries, std::map<std::string, void*>& registered_dl_handlers, std::map<std::string, void*>& registered_functions);
    ~HotpatchCommand();

    void ParseCommand(std::string command);

    bool HandleCommand(std::vector<std::string> command);

    bool HandleGflagsList();
    bool HandleGflagsGet(string key);
    bool HandleGflagsSet(string key, string value);
    bool HandleVarList();
    bool HandleVarGet(string type, string key);
    bool HandleVarSet(string type, string key, string value);
    bool HandleLibList();
    bool HandleLibLoad(string name, string path);
    bool HandleLibUnload(string name);
    bool HandleFuncUpgrade(string lib_name, string func_name);
    bool HandleFuncRollback(string func_name);

};

} // End of namespace


#endif
