#define _GNU_SOURCE     /* for RTLD_NEXT */
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <dlfcn.h>
#include <err.h>
//#include "hotpatch_server.h"

#ifdef __UCLIBC__
#define __libc_start_main       __uClibc_main
#endif

#define STR_(s) #s
#define STR(s) STR_(s)

/*
cc -fPIC -shared -Wall -W -Wno-parentheses preload_hotpatch.c -o libpreload_hotpatch.so -ldl
LD_PRELOAD="./libpreload_hotpatch.so" /bin/echo a b c d
*/

typedef int (*__libc_start_main_type)(
        int (*main)(int,char**,char**), int ac, char **av,
        void (*init)(void), void (*fini)(void),
        void (*rtld_fini)(void), void *stack_end);


int __libc_start_main(
        int (*main)(int,char**,char**), int ac, char **av,
        void (*init)(void), void (*fini)(void),
        void (*rtld_fini)(void), void *stack_end) {
            
        //typeof(__libc_start_main) *real_lsm;
        __libc_start_main_type real_lsm = (__libc_start_main_type)dlsym(RTLD_NEXT, STR(__libc_start_main));

        printf("---------- Welcome to hotpatch ----------\n");
        //hotpatch::InitHotpatchServer();

        return real_lsm(main, ac, av, init, fini, rtld_fini, stack_end);
}