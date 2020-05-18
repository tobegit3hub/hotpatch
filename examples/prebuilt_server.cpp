#include <iostream>
#include <chrono>
#include <thread>
#include <gflags/gflags.h>
#include <glog/logging.h>

using namespace std;

DEFINE_bool(debug, true, "Use debug or not");
DEFINE_string(log_level, "debug,info,warn", "The log level");

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

    for(int i=0; i<10; i++) {
        cout << "Sleep for one second" << endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));

        cout << "Config debug: " << FLAGS_debug << endl;
        cout << "Config log_level: " << FLAGS_log_level << endl;

        LOG(INFO) << "glog debug message";
        LOG(WARNING) << "glog warning message";
        LOG(ERROR) << "glog erro message";

        add_func(1, 2);
    }

}

