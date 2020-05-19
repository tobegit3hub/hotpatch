#include<stdio.h>
#include "hotpatch_server.h"

// LD_PRELOAD="./libpreload_hotpatch_linux.so" /bin/echo a b c d
static void __attribute__((constructor)) ctor() {
    printf("---------- Welcome to hotpatch ----------\n");

    // This requires to build the shared library with "-fPIC"
    hotpatch::InitHotpatchServer();
}
