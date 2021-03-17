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
`scalable_allocator` in C++17 parallel algorithms.
## Building from Source
`scalable_allocators` can be built using [CMake](https://cmake.org/) on multiple
platforms (Linux/Windows/MacOS).
### Prerequisites
* [CMake](https://cmake.org/) version 3.15 or higher
* C++ compiler supporting the `C++17` standard (e.g. GCC 9.3)

### Third-party libraries:

* [Intel's Threading Building Block (now OneTBB)](https://github.com/oneapi-src/oneTBB)
* [Boost C++ libraries](https://www.boost.org/) 1.70 or higher. It is only
   necessary if you want to build unit tests. Just `boost.system` and
   `boost.test` would suffice.

### Optional third-party libraries
* [Google Benchmark](https://github.com/google/benchmark)

The `CMake` script configured in a way that if it cannot find the optional
third-party libraries it tries to download and build them automatically. So,
there is no need to do anything if they are missing but you need a
working internet connection.
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
## Running unit tests
After building the program successfully, you can run the unit tests by typing
`ctest` or `make test`:
```
$ ctest
Running 3 test cases...

*** No errors detected
```
## Running benchmarks
After buliding, you will find all the benchmarks in `build/src` dirctory:
```
$ src/perf_rmsd
2021-03-16T22:40:24-04:00
Running /home/asobhani/development/scalable_allocators/build/src/perf_rmsd
Run on (40 X 3700 MHz CPU s)
CPU Caches:
  L1 Data 32 KiB (x40)
  L1 Instruction 32 KiB (x40)
  L2 Unified 1024 KiB (x40)
  L3 Unified 28160 KiB (x2)
Load Average: 2.08, 1.99, 2.26
------------------------------------------------------------------
Benchmark                        Time             CPU   Iterations
------------------------------------------------------------------
RMSD/seq/1048576              3.87 ms         3.86 ms          181
RMSD/seq/2097152              8.48 ms         8.46 ms           88
RMSD/seq/4194304              16.7 ms         16.6 ms           42
RMSD/seq/8388608              32.5 ms         32.4 ms           22
RMSD/seq/16777216             69.2 ms         69.1 ms           11
RMSD/par/1048576             0.165 ms        0.164 ms         4117
RMSD/par/2097152             0.329 ms        0.328 ms         1784
RMSD/par/4194304             0.978 ms        0.976 ms          715
RMSD/par/8388608              2.39 ms         2.38 ms          277
RMSD/par/16777216             5.35 ms         5.33 ms          123
RMSD_TBBSA/seq/1048576        3.89 ms         3.88 ms          186
RMSD_TBBSA/seq/2097152        7.84 ms         7.82 ms           91
RMSD_TBBSA/seq/4194304        15.6 ms         15.5 ms           45
RMSD_TBBSA/seq/8388608        31.6 ms         31.5 ms           22
RMSD_TBBSA/seq/16777216       62.6 ms         62.5 ms           11
RMSD_TBBSA/par/1048576       0.167 ms        0.166 ms         4282
RMSD_TBBSA/par/2097152       0.363 ms        0.360 ms         1950
RMSD_TBBSA/par/4194304       0.990 ms        0.985 ms          721
RMSD_TBBSA/par/8388608        2.38 ms         2.37 ms          291
RMSD_TBBSA/par/16777216       5.29 ms         5.26 ms          127
```
Using `--help` shows all the available options for running the benchmark:

```
$ src/perf_rmsd --help
benchmark [--benchmark_list_tests={true|false}]
          [--benchmark_filter=<regex>]
          [--benchmark_min_time=<min_time>]
          [--benchmark_repetitions=<num_repetitions>]
          [--benchmark_report_aggregates_only={true|false}
          [--benchmark_format=<console|json|csv>]
          [--benchmark_out=<filename>]
          [--benchmark_out_format=<json|console|csv>]
          [--benchmark_color={auto|true|false}]
          [--benchmark_counters_tabular={true|false}]
          [--v=<verbosity>]
```

## Interpreting the Results
The benchmark is tested a couple of times with different iteration counts to determine the correct count to use. After it finds an iteration count that causes the benchmark to run for a significant enough period of time it reports only that run in the last column (*Iterations*), and not the test runs before it. The *Time* is the average wall time per iteration. The *CPU* is the average CPU time per iteration. By default the *CPU* time is used when determining the amount of iterations to run.
