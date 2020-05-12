#ifndef _H_HOTPATCH_SERVER
#define _H_HOTPATCH_SERVER

namespace hotpatch {

class HotpatchServer {

private:
    std::thread socket_server_thread;
    bool should_stop = false;

public:
    HotpatchServer();

    ~HotpatchServer();

    void init();

    void close();

};

} // End of namespace

#endif