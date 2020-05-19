#include <stdio.h>
#include <syslog.h>

#include "hotpatch_server.h"

/*
For Linux, LD_PRELOAD="./libpreload_hotpatch.so" /bin/echo foo
For MacOS, DYLD_INSERT_LIBRARIES=./libpreload_hotpatch.dylib /bin/echo foo
*/
__attribute__((constructor))
static void customConstructor(int argc, const char **argv) {
     printf("---------- Welcome to hotpatch ----------\n");
     hotpatch::InitHotpatchServer();
}