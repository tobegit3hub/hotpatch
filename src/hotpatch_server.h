#ifndef _H_HOTPATCH_SERVER
#define _H_HOTPATCH_SERVER

#include <map>
#include <string>
#include <thread>

#include "hotpatch_command.h"

using namespace std;

namespace hotpatch {

extern void InitHotpatchServer();
extern void ShutdownHotpatchServer();
extern void RegisterVariable(std::string key, void *p_value);
extern void RegisterFunction(std::string name, void *p_func);

class HotpatchServer {

private:
    std::thread socket_server_thread;
    bool should_stop = false;

    std::map<std::string, void*> registered_variables;
    std::map<std::string, void*> registered_libraries;
    std::map<std::string, void*> registered_dl_handlers;

    // Map of function name and function pointer which casts to void*
    std::map<std::string, void*> registered_functions;


    // TODO: Support multiple handlers for more libraries
    void* dl_handler;
    std::shared_ptr<HotpatchCommand> hotpatch_command;

public:

    HotpatchServer();

    ~HotpatchServer();

    void Init();

    void Close();

    void RegisterVariable(std::string key, void *p_value);

    void RegisterFunction(std::string func_name, void* p_func);

    bool GetShouldStop();

    void SetShouldStop(bool stop);

    std::shared_ptr<HotpatchCommand> GetHotpathCommand();

};

} // End of namespace

#endif