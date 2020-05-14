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

//#define WRAPPER(FUNC) wrapper<decltype(&FUNC), &FUNC>


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

    gflags::ParseCommandLineFlags(&argc, &argv, true);
    // Use glog
    FLAGS_logtostderr = 1;
    google::InitGoogleLogging(argv[0]); 

    FLAGS_minloglevel = 0;

    auto hp = make_shared<hotpatch::HotpatchServer>();
    hp->Init();

    // TODO: Provide static method to register variables
    // Register variable
    string user_name = "myname";
    hp->RegisterVariable("user_name", &user_name);

   //auto p_add_func = (decltype(add_func)*) hp->RegisterFunction("add_func", reinterpret_cast<void*>(add_func));

    for(int i=0; i<10; i++) {
        cout << "Sleep for one second" << endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));

        cout << "Config debug: " << FLAGS_debug << endl;
        cout << "Config log_level: " << FLAGS_log_level << endl;

        cout << "User name: " << user_name << endl;

        //p_add_func(1, 2);

        //auto result = WRAPPER(add_func)(10, 20);
        auto result = wrapper<decltype(&add_func), &add_func>(10, 20);
        cout << "Wrapper function result: " << result << endl;


        LOG(INFO) << "glog debug message";
        LOG(WARNING) << "glog warning message";
        LOG(ERROR) << "glog erro message";
    }

    hp->Close();

}

