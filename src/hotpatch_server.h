#ifndef _H_HOTPATCH_SERVER
#define _H_HOTPATCH_SERVER

#include <map>
#include <string>

namespace hotpatch {

class HotpatchServer {

private:
    std::thread socket_server_thread;
    bool should_stop = false;
    std::map<std::string, void*> registered_variables;

public:
    HotpatchServer();

    ~HotpatchServer();

    void init();

    void close();

    void register_variable(std::string key, void *p_value);


};

} // End of namespace

#endif