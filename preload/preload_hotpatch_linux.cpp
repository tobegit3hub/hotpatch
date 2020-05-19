#include<stdio.h>

// LD_PRELOAD="./libpreload_hotpatch_linux.so" /bin/echo a b c d
static void __attribute__((constructor)) ctor() {
    printf("---------- Welcome to hotpatch ----------\n");
}
