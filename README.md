# scalable_allocators
In a multithreaded C/C++ program, using standard non-threaded allocators, memory
allocation can become a bottleneck. That is firstly caused by thread competition
for a lock on a shared global heap, and secondly for caching effects. Programs
that run this way are not scalable and may slow down as the number of cores
increases. Scalable memory allocators such as
[Intel’s TBB allocators](https://github.com/oneapi-src/oneTBB),
[FreeBSD’s jemalloc](https://github.com/jemalloc/jemalloc) and
[Google’s TCMalloc](https://github.com/google/tcmalloc) solve this problem by
providing various optimizations such as *per-CPU caches*, *thread-private heaps*,
*sized deletes* and *fast/slow path improvements*. You can easily gain a 20-30%
performance improvement for parallel sections and even 4X in extreme cases by
simply relinking with a scalable memory allocator. 

`scalable_allocators` is a benchmark for using
[Intel TBB's](https://github.com/oneapi-src/oneTBB)
`scalable_allocator` in C++/17 parallel algorithms.
## Building from Source
`scalable_allocators` can be built using [CMake](https://cmake.org/) on multiple
platforms (Linux/Windows/MacOS).
### Prerequisites
* [CMake](https://cmake.org/) version 3.15 or higher
* C++ compiler supporting the `C++17` standard (e.g. GCC 9.3)

### Third-party libraries:

* [Boost C++ libraries](https://www.boost.org/) 1.70 or higher. It is only
   necessary if you want to build unit tests. Just `boost.system` and
   `boost.test` would suffice.

### Optional third-party libraries
* [Google Benchmark](https://github.com/google/benchmark)

The `CMake` script configured in a way that if it cannot find the optional
third-party libraries it tries to download and build them automatically. So,
there is no need to do anything if they are missing.
### Building using CMake
On all platforms (Linux/Windows/MacOS), you can use the following commands:
```
$ git clone https://github.com/arminms/scalable_allocators.git
$ cd scalable_allocators
$ mkdir build && cd build
$ cmake ..
$ cmake --build . --config Release
```
## Building in VSCode
First, you need the following [VSCode](https://code.visualstudio.com/) extensions
if you haven't installed them yet:
* [C/C++](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cpptools)
* [CMake Tools](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cmake-tools)

Alternatively, you can install
[C/C++ Extension Pack](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cpptools-extension-pack)
that has all the necessary extensions along with other useful ones.

The best way to build/run/debug on all platforms (Linux/Windows/MacOS) within [VSCode](https://code.visualstudio.com/)
is using [vcpkg](https://docs.microsoft.com/en-us/cpp/build/vcpkg). After installing
`vcpkg`, you can install necessary libraries using the following
command:
```
$ ./vcpkg install tbb benchmark boost-system boost-test
```
Then in `VSCode` clone the repository from https://github.com/arminms/scalable_allocators.git
and wait till the CMake configuration finishes. Now, you can build/run/debug using
the buttons provided by `CMake Tools` extension on the status bar.
## Running Unit Tests
After building the program successfully, you can run the unit tests by typing
`ctest` or `make test`.
