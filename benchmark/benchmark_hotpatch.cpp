
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

int fibonacci(int n) {
    if (n <= 1) {
        return n;
    } else {
        return fibonacci(n-1) + fibonacci(n-2);
    }
}

int main(int argc, char **argv) {
    const int BENCHMARK_ITERATION = 1;
    timestamp_t start;
    timestamp_t end;
    double duration;

    auto hotpatch_server = std::make_unique<hotpatch::HotpatchServer>();
    hotpatch_server->Init();

    // Test native C++ function
    printf("start");
    start = get_timestamp();
    for (int i=0; i<BENCHMARK_ITERATION; ++i) {
        fibonacci(45);
    }
    printf("end");
    end = get_timestamp();
    duration = (end - start) / 1000000.0L;
    cout << "Native call time: " << duration << endl;

    // Register and upgrade to new function implementation
    hotpatch_server->RegisterFunction("fibonacci", (void*)fibonacci);
    hotpatch_server->GetHotpathCommand()->HandleLibLoad("fibonacci_patch1", "./examples/libfibonacci_patch1.so");
    hotpatch_server->GetHotpathCommand()->HandleFuncUpgrade("fibonacci_patch1", "fibonacci");

    // Test hotpatch C++ function
    start = get_timestamp();
    for (int i=0; i<BENCHMARK_ITERATION; ++i) {
        fibonacci(45);
    }
    end = get_timestamp();
    duration = (end - start) / 1000000.0L;
    cout << "Hotpatch function call time: " << duration << endl;

    return 0;
}




