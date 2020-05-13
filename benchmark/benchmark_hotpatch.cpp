
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

typedef int (*add_one_type)(int);

void* mock_register_function(std::string func_name, void* p_function){
    return p_function;
}

int main(int argc, char **argv) {

    auto hp = make_shared<hotpatch::HotpatchServer>();
    hp->Init();

    /*
        Native call time: 2.17616
        Use native function point time: 2.35947
        Cast native function time: 2.36825
        Mock register native function time: 2.36832
        Hotpatch call native time: 2.44159
    */

    // Test native C++ function
    timestamp_t start = get_timestamp();
    const int BENCHMARK_ITERATION = 1000000000;
    for (int i=0; i<BENCHMARK_ITERATION; ++i) {
        add_one(1);
    }
    timestamp_t end = get_timestamp();
    double secs = (end - start) / 1000000.0L;
    cout << "Native call time: " << secs << endl;

    // Test using function pointer
    add_one_type p_add_one = add_one;
    start = get_timestamp();
    for (int i=0; i<BENCHMARK_ITERATION; ++i) {
        p_add_one(1);
    }
    end = get_timestamp();
    secs = (end - start) / 1000000.0L;
    cout << "Use native function point time: " << secs << endl;


    // Test cast function pointer
    p_add_one = (add_one_type) reinterpret_cast<void*>(add_one);
    start = get_timestamp();
    for (int i=0; i<BENCHMARK_ITERATION; ++i) {
        p_add_one(1);
    }
    end = get_timestamp();
    secs = (end - start) / 1000000.0L;
    cout << "Cast native function time: " << secs << endl;


    // Test mock register function pointer
    p_add_one = (add_one_type) mock_register_function("", reinterpret_cast<void*>(add_one));
    start = get_timestamp();
    for (int i=0; i<BENCHMARK_ITERATION; ++i) {
        p_add_one(1);
    }
    end = get_timestamp();
    secs = (end - start) / 1000000.0L;
    cout << "Mock register native function time: " << secs << endl;


    // Test register native function before calling
    const string func_name = "add_one";
    p_add_one = (add_one_type) hp->RegisterFunction(func_name, reinterpret_cast<void*>(add_one));
    start = get_timestamp();
    for (int i=0; i<BENCHMARK_ITERATION; ++i) {
        p_add_one(1);
    }
    end = get_timestamp();
    secs = (end - start) / 1000000.0L;
    cout << "Hotpatch call native time: " << secs << endl;


    hp->Close();

    return 0;
}




