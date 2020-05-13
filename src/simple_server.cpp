#include <iostream>
#include <chrono>
#include <thread>
#include <gflags/gflags.h>
#include <glog/logging.h>

#include "hotpatch_server.h"

using namespace std;


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
    FLAGS_logtostderr = 1;
    google::InitGoogleLogging(argv[0]); 

    LOG(INFO) << "glog info message";

    auto hp = make_shared<hotpatch::HotpatchServer>();
    hp->Init();

    // TODO: Provide static method to register variables
    // Register variable
    string user_name = "myname";
    hp->RegisterVariable("user_name", &user_name);

    auto p_add_func = (decltype(add_func)*) hp->RegisterFunction("add_func", reinterpret_cast<void*>(add_func));

    for(int i=0; i<10; i++) {
        cout << "Sleep for one second" << endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));

        cout << "Config debug: " << FLAGS_debug << endl;
        cout << "Config log_level: " << FLAGS_log_level << endl;

        cout << "User name: " << user_name << endl;

        p_add_func(1, 2);
    }

    hp->Close();

}

