#ifndef _H_HOTPATCH_SERVER
#define _H_HOTPATCH_SERVER

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

#endif