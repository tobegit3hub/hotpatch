#include <stdio.h>
#include <syslog.h>

#include "hotpatch_server.h"

/*
DYLD_INSERT_LIBRARIES=./libpreload_hotpatch_mac.dylib /bin/echo ss
*/
__attribute__((constructor))
static void customConstructor(int argc, const char **argv) {
     printf("---------- Welcome to hotpatch ----------\n");
     hotpatch::InitHotpatchServer();

     syslog(LOG_ERR, "Dylib injection successful in %s\n", argv[0]);
}