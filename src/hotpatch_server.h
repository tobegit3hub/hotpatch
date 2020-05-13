#ifndef _H_HOTPATCH_SERVER
#define _H_HOTPATCH_SERVER

#include <map>
#include <string>
#include <thread>

namespace hotpatch {

typedef void (*void_function_type)();

class HotpatchServer {

private:
    std::thread socket_server_thread;
    bool should_stop = false;
    std::map<std::string, void*> registered_variables;

    // TODO: Support multiple handers for more libraries
    void* dl_handler;

public:
    HotpatchServer();

    ~HotpatchServer();

    void Init();

    void Close();

    void RegisterVariable(std::string key, void *p_value);

    void* RegisterFunction(std::string func_name, void* p_function);
    
    bool GetShouldStop();

    void SetShouldStop(bool stop);
};

} // End of namespace

#endif