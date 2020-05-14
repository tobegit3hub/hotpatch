#ifndef _H_HOTPATCH_COMMAND
#define _H_HOTPATCH_COMMAND

#include <string>

using namespace std;

namespace hotpatch {

class HotpatchCommand {

private:
public:

    HotpatchCommand();
    ~HotpatchCommand();

    void ParseCommand(std::string command);

    bool HandleCommand(std::vector<std::string> command);

    bool HandleGflagsList();
    bool HandleGflagsGet(string key);
    bool HandleGflagsSet(string key, string value);
    bool HandleVarList();
    bool HandleVarGet(string key);
    bool HandleVarSet(string key, string value);
    bool HandleLibList();
    bool HandleLibLoad(string name, string path);
    bool HandleFuncList();
    bool HandleFuncLoad(string lib, string name);

};

} // End of namespace


#endif