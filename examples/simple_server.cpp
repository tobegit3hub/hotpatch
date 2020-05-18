#include <iostream>
#include <chrono>
#include <thread>
#include <gflags/gflags.h>
#include <glog/logging.h>
#include <dlfcn.h>
#include <memory>
#include <cassert>

#include "hotpatch_server.h"

using namespace std;


template<typename Fn, Fn fn, typename... Args>
typename std::result_of<Fn(Args...)>::type wrapper(Args&&... args) {

    /*
    const std::string lib_name = "../examples/libadd_func_patch1.dylib";
    void* dl_handler = dlopen(lib_name.c_str(), RTLD_LAZY);

    std::string func_name = "add_func";
    auto new_fun = (decltype(fn)) dlsym(dl_handler, func_name.c_str());

    return new_fun(std::forward<Args>(args)...);
    */
    return fn(std::forward<Args>(args)...);
}

// Use gflags
DEFINE_bool(debug, true, "Use debug or not");
DEFINE_string(log_level, "debug,info,warn", "The log level");

// Define local function
int add_func(int a, int b) {
    cout << "Call simple add" << endl;
    int result = a + b;
    cout << "Result: " << result << endl;
    return result;
}

int main(int argc, char **argv) {
    // Use gflags
    gflags::ParseCommandLineFlags(&argc, &argv, true);

    // Use glog
    google::InitGoogleLogging(argv[0]); 
    FLAGS_logtostderr = 1;
    FLAGS_minloglevel = 0;

    // Use Hotpatch
    hotpatch::InitGlobalHotpatchServer();

    // Register variable
    std::string user_name = "myname";
    hotpatch::RegisterVariable("user_name", &user_name);

    int age = 10;
    hotpatch::RegisterVariable("age", &age);

    hotpatch::RegisterFunction("add_func", reinterpret_cast<void*>(add_func));

    // Run serving logic
    for(int i=0; i<100; i++) {
        cout << "Sleep for one second" << endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));

        cout << "Config debug: " << FLAGS_debug << endl;
        cout << "Config log_level: " << FLAGS_log_level << endl;
        cout << "User name: " << user_name << endl;

        LOG(INFO) << "glog debug message";
        LOG(WARNING) << "glog warning message";
        LOG(ERROR) << "glog erro message";

        add_func(1, 2);
    }

}

