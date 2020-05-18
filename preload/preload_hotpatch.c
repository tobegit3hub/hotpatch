#define _GNU_SOURCE     /* for RTLD_NEXT */
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <dlfcn.h>
#include <err.h>
#ifdef __UCLIBC__
#define __libc_start_main       __uClibc_main
#endif
#define STR_(s) #s
#define STR(s)  STR_(s)

/*
cc -fPIC -shared -Wall -W -Wno-parentheses preload_hotpatch.c -o skip1.so -ldl
LD_PRELOAD="./skip1.so" /bin/echo a ss dd b c d
*/

int __libc_start_main(
        int (*main)(int,char**,char**), int ac, char **av,
        int (*init)(int,char**,char**), void (*fini)(void),
        void (*rtld_fini)(void), void *stack_end)
{
        typeof(__libc_start_main) *real_lsm;

        if(*(void**)&real_lsm = dlsym(RTLD_NEXT, STR(__libc_start_main))) {
            printf("---------- Welcome to hotpatch ----------\n");
            return real_lsm(main, ac, av, init, fini, rtld_fini, stack_end);
        } else {
            errx(1, "BUG: dlsym: %s", dlerror());
        }
}