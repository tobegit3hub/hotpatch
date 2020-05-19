
#include <sys/time.h>
#include <iostream>
#include <chrono>
#include <thread>

#include "hotpatch_server.h"

using namespace std;

typedef unsigned long long timestamp_t;
static timestamp_t get_timestamp () {
    struct timeval now;
    gettimeofday (&now, NULL);
    return  now.tv_usec + (timestamp_t)now.tv_sec * 1000000;
}

int add_one(int a) {
    return a+1;
}

int main(int argc, char **argv) {
    const int BENCHMARK_ITERATION = 1000000000;
    timestamp_t start;
    timestamp_t end;
    double duration;

    auto hotpatch_server = std::make_unique<hotpatch::HotpatchServer>();
    hotpatch_server->Init();

    // Test native C++ function
    start = get_timestamp();
    for (int i=0; i<BENCHMARK_ITERATION; ++i) {
        add_one(1);
    }
    end = get_timestamp();
    duration = (end - start) / 1000000.0L;
    cout << "Native call time: " << duration << endl;

    // Register and upgrade to new function implementation
    hotpatch_server->RegisterFunction("add_one", (void*)add_one);
    hotpatch_server->GetHotpathCommand()->HandleLibLoad("add_one_lib", "./examples/libadd_one_patch1.so");
    hotpatch_server->GetHotpathCommand()->HandleFuncUpgrade("add_one_lib", "add_one");

    //std::this_thread::sleep_for(std::chrono::milliseconds(5000));

    // Test hotpatch C++ function
    start = get_timestamp();
    for (int i=0; i<BENCHMARK_ITERATION; ++i) {
        add_one(1);
    }
    end = get_timestamp();
    duration = (end - start) / 1000000.0L;
    cout << "Hotpatch function call time: " << duration << endl;

    return 0;
}




