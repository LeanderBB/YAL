YAL - Yet Another Language
==========================

A toy language implementation to delve into the topic of compilers and virtual machines.

YAL is a language that is statically typed with auto inference. It takes some clues from languagues such as Google's GO, Python, C, Swift/Objective-C and Lua.

YAL will initially transpile itself to C89 or C99 source files. An LLVM backend might be considered in the future.

Since this is still at early development stages, the language itself is subject to continuous change.

Building
--------

YAL depends on:
* Lemon Parser 1.0 (If not found, pre-generated source file will be used)
* Re2c 1.0.2 (If not found, pre-generated source file will be used)
* GTest (When Testing Enabled)

YAL has been build and tested on:
* GNU/Linux x86_64 

The project can be built using the standard CMake build process.
```
cmake -DCMAKE_BUILD_TYPE=Release <source directory>
cmake --build <build directory> --config=Release 
```
