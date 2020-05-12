#include <iostream>
#include <chrono>
#include <thread>
#include <gflags/gflags.h>


#include "hotpatch_server.h"

using namespace std;


DEFINE_bool(debug, true, "Use debug or not");
DEFINE_string(log_level, "debug,info,warn", "The log level");

typedef int (*function_type)(int, int);

int simple_add(int a, int b) {
    cout << "Call simple add" << endl;
    int result = a + b;
    cout << "Result: " << result << endl;
    return result;
}


int main(int argc, char **argv) {

    gflags::ParseCommandLineFlags(&argc, &argv, true);

    cout << "Start main" << endl;

    auto hp = make_shared<hotpatch::HotpatchServer>();

    hp->init();

    // Register variable
    string user_name = "test_name";
    hp->register_variable("user_name", &user_name);

    // Register function
    const string func_name = "new_add_func";
    function_type p_simple_add = (function_type) hp->register_function(func_name, reinterpret_cast<void*>(simple_add));

    for(int i=0; i<10; i++) {
        cout << "Sleep for one second" << endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));

        cout << "Config debug: " << FLAGS_debug << endl;
        cout << "Config log_level: " << FLAGS_log_level << endl;

        cout << "User name: " << user_name << endl;

        p_simple_add(1, 2);
    }


    hp->close();


    cout << "End of main" << endl;

}

