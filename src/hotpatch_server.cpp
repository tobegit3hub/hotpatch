
#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h> 
#include <string>
#include <sys/types.h>
#include <iostream>
#include <thread>
#include <gflags/gflags.h>
#include <string>
#include <dlfcn.h>
#include <memory>
#include <cassert>

#include "hotpatch_server.h"

namespace hotpatch {

HotpatchServer::HotpatchServer() {

}

HotpatchServer::~HotpatchServer() {

}

void start_socket_server() {

    const int buffer_size = 128;
    const int client_size = 8;

    // Get pid to set socket file
    pid_t pid = getpid();
    // TODO: Set for test
    pid = 100;
    std::string socket_path = "/tmp/" + std::to_string(pid) + ".socket";
    std::cout << "Unix socket path: " << socket_path << std::endl;

    struct sockaddr_un serun;
    int listenfd, size;

    if ((listenfd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0) {
        perror("socket error");
        exit(1);
    }
 
    memset(&serun, 0, sizeof(serun));
    serun.sun_family = AF_UNIX;
    strcpy(serun.sun_path, socket_path.c_str());
    size = offsetof(struct sockaddr_un, sun_path) + strlen(serun.sun_path);
    unlink(socket_path.c_str());

    if (bind(listenfd, (struct sockaddr *)&serun, size) < 0) {
        perror("bind error");
        exit(1);
    }

    // Start unix socket server    
    if (listen(listenfd, client_size) < 0) {
        perror("listen error");
        exit(1);        
    }

    struct sockaddr_un cliun;
    socklen_t cliun_len = sizeof(cliun);
    char read_buf[buffer_size], write_buf[buffer_size];
    int connfd;
    while(1) {
        // Accept socket client
        if ((connfd = accept(listenfd, (struct sockaddr *)&cliun, &cliun_len)) < 0){
            perror("accept error");
            continue;
        }
        
        // TODO: Pass the flag to stop waiting clients
        while(1) {
            int readSize = read(connfd, read_buf, sizeof(read_buf));
            if (readSize < 0) {
                perror("read error");
                break;
            } else if(readSize == 0) {
                printf("EOF\n");
                break;
            }

            // Handle command
            std::cout << "Receive command: " + std::string(read_buf) << std::endl;
 
            // TODO: Parse user command to set key-value
            auto result = gflags::SetCommandLineOption("log_level", "test_log_level");
            std::cout << "Set gflags result: " << result << std::endl;

            // TODO: Get registered variables to set

            // Output result
            std::string sendResult = "Success to run";
            std::cout << sendResult << std::endl;
            write(connfd, sendResult.c_str(), sendResult.size());
        }
        close(connfd);
    }
    close(listenfd);
}

void HotpatchServer::init() {

    socket_server_thread = std::thread(start_socket_server);

    const std::string lib_name = "../examples/libnew_add_func.dylib";

    dl_handler = dlopen(lib_name.c_str(), RTLD_LAZY);
    if (!dl_handler) {
        std::cerr << "Cannot open library: " << dlerror() << '\n';
    }

}

void HotpatchServer::close() {

    dlclose(dl_handler);

    socket_server_thread.join();

}

void HotpatchServer::register_variable(std::string key, void *p_value) {
    // TODO: Make share to release smart pointers
    registered_variables[key] = p_value;

    // TODO: Test to set registered variable
    registered_variables["user_name"] = new std::string("new_name");
}


void* HotpatchServer::register_function(std::string func_name, void* p_function) {
    // TODO: Change function pointer with new implementation
    //return dlsym(dl_handler, func_name.c_str());
    return p_function;
}


} // End of namespace