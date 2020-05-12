#include <iostream>
#include <chrono>
#include <thread>
#include <gflags/gflags.h>

#include "hotpatch_server.h"

using namespace std;


DEFINE_bool(debug, true, "Use debug or not");
DEFINE_string(log_level, "debug,info,warn", "The log level");


int main(int argc, char **argv) {

    gflags::ParseCommandLineFlags(&argc, &argv, true);

    cout << "Start main" << endl;

    auto hp = make_shared<hotpatch::HotpatchServer>();

    hp->init();


    for(int i=0; i<10; i++) {
        cout << "Sleep for one second" << endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));

        cout << "Config debug: " << FLAGS_debug << endl;
        cout << "Config log_level: " << FLAGS_log_level << endl;
    }


    hp->close();

    cout << "End of main" << endl;

}

