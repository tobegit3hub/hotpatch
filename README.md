# HotPatch

## Introduction

Computer progroms are like humans in a way which is extensible to do almost everything. But human can learn and react continuously while softwares need to restart after adding features and applying patches. In some cases, we can apply the hot patches which are different from the static code patches without restarting the processes.

This project helps to patch and upgrade your programs with the following features.

- [x] Set and get the [gflags](https://github.com/gflags/gflags) on the fly.
- [x] Set the logging properties of [glogs](https://github.com/google/glog) on the fly.
- [x] Set and print any registered variables on the fly.
- [x] Load and unload the dynamic libraries on the fly.
- [x] Upgrade and rollback the implementation of registered functions on the fly.

## Installation

Build the project from scratch. Required to install `gflags`, `glogs`, `gtest` and `subhook` in advance.

```
mkdir ./build/
cd ./build/
cmake ..
make
```

Integrated with your C++ programs.

```
cmake -DCMAKE_INSTALL_PREFIX=/foo/bar/ ..
make
make install
```

## Quickstart

We can run any pre-built binary with the preload library to change the properties at any time.

```
# For Linux
LD_PRELOAD="./preload/libpreload_hotpatch.so" ./examples/prebuilt_server

# For Mac
DYLD_INSERT_LIBRARIES=./preload/libpreload_hotpatch.dylib ./examples/prebuilt_server
```

Use the Python client or command `nc` to set and get the log level on the fly.

```
hotpatch -p $PID gflags set minloglevel 2

hotpatch -p $PID gflags get minloglevel
```

We can register the variables and functions in your C++ program, take [simple_server](./examples/simple_server.cpp) for example.

```
// Use Hotpatch
hotpatch::InitHotpatchServer();

// Register variable
std::string user_name = "myname";
hotpatch::RegisterVariable("user_name", &user_name);
int age = 10;
hotpatch::RegisterVariable("age", &age);

// Register function
hotpatch::RegisterFunction("add_func", reinterpret_cast<void*>(add_func));
```

Then we can get and set the values in any time while the program is running. It is meaningful for debuging and changing the logic of your programs without re-compiling and restarting the process.

```
hotpatch -p $PID var get string user_name
hotpatch -p $PID var set string user_name new_name

hotpatch -p $PID var get int age
hotpatch -p $PID var set int age 20
```

Loading the new libraries allows developers to inject the new implementation of the functions on the fly and rollback is supported as well.

```
hotpatch -p $PID lib load add_func_patch1 ./examples/libadd_func_patch1.dylib

hotpatch -p $PID func upgrade add_func_patch1 add_func

hotpatch -p $PID func rollback add_func
```

## Clients

### Python Client

The command `hotpatch` is the socket client in Python which can be installed easily.

```
pip install hotpatch
```

Get and set the `gflags` or `glogs` properties.

```
hotpatch -p $PID gflags list

hotpatch -p $PID gflags get minloglevel

hotpatch -p $PID gflags set minloglevel 2
```

Get and set the registered variables.

```
hotpatch -p $PID var list

hotpatch -p $PID var get string user_name

hotpatch -p $PID var set string user_name new_name
```

Load and unload the dynamic libraries.

```
hotpatch -p $PID lib list

hotpatch -p $PID lib load add_func_patch1 ./examples/libadd_func_patch1.dylib

hotpatch -p $PID lib unload add_func_patch1
```

Upgrade and rollback the function implementation.

```
hotpatch -p $PID func upgrade add_func_patch1 add_func

hotpatch -p $PID func rollback add_func
```

### Shell Client

`nc` is the built-in tool in most operating systems which can access the unix socket as well.

Get and set the `gflags` or `glogs` properties.

```
echo "gflags list" | nc -U /tmp/$PID.socket

echo "gflags get minloglevel" | nc -U /tmp/$PID.socket

echo "gflags set minloglevel 2" | nc -U /tmp/$PID.socket
```

Get and set the registered variables.

```
echo "var list" | nc -U /tmp/$PID.socket

echo "var get string user_name" | nc -U /tmp/$PID.socket

echo "var set string user_name new_name" | nc -U /tmp/$PID.socket
```

Load and unload the dynamic libraries.

```
echo "lib list" | nc -U /tmp/$PID.socket

echo "lib load add_func_patch1 ./examples/libadd_func_patch1.dylib" | nc -U /tmp/$PID.socket

echo "lib unload add_func_patch1" | nc -U /tmp/$PID.socket
```

Upgrade and rollback the function implementation.

```
echo "func upgrade add_func_patch1 add_func" | nc -U /tmp/$PID.socket

echo "func rollback add_func" | nc -U /tmp/$PID.socket
```

## Performance

**No performance overhead for your application!**

Under the hood, `gflags` manages all the flag information globally and provide the APIs to access. For registered variables, we use the void pointer to access and only support for primitive types so far. For registered functions, the underlying instructions are mofified to jump to the new function address which brings no performance degradation. Here is the underlying implementation of [subhook](https://github.com/Zeex/subhook) for x86 architecture.

```
static int subhook_make_jmp64(void *src, void *dst) {
  struct subhook_jmp64 *jmp = (struct subhook_jmp64 *)src;

  jmp->push_opcode = PUSH_OPCODE;
  jmp->push_addr = (uint32_t)(uintptr_t)dst; /* truncate */
  jmp->mov_opcode = MOV_OPCODE;
  jmp->mov_modrm = JMP64_MOV_MODRM;
  jmp->mov_sib = JMP64_MOV_SIB;
  jmp->mov_offset = JMP64_MOV_OFFSET;
  jmp->mov_addr = (uint32_t)(((uintptr_t)dst) >> 32);
  jmp->ret_opcode = RET_OPCODE;

  return 0;
}
```

You can get more benchmark results with the scripts in [benchmark](./benchmark/).

## Notice

For MacOS Catalina users, you need to set `printf '\x07' | dd of=<executable> bs=1 seek=160 count=1 conv=notrunc` because of the [security issue](https://stackoverflow.com/questions/60654834/using-mprotect-to-make-text-segment-writable-on-macos).
