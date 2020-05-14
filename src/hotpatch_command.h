#ifndef _H_HOTPATCH_COMMAND
#define _H_HOTPATCH_COMMAND

#include <string>

namespace hotpatch {
class HotpatchCommand {

private:
public:

    HotpatchCommand();
    ~HotpatchCommand();

    void ParseCommand(std::string command);
};

} // End of namespace


#endif