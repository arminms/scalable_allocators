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
## Running unit tests
After building the program successfully, you can run the unit tests by typing
`ctest` or `make test`.
## Running benchmarks
After buliding, you will find all the benchmarks in `build/src` dirctory:
```
$ src/perf_rmsd
2021-03-15T23:17:19-04:00
Running src/perf_rmsd
Run on (40 X 3700 MHz CPU s)
CPU Caches:
  L1 Data 32 KiB (x40)
  L1 Instruction 32 KiB (x40)
  L2 Unified 1024 KiB (x40)
  L3 Unified 28160 KiB (x2)
Load Average: 0.56, 0.83, 1.21
------------------------------------------------------------------
Benchmark                        Time             CPU   Iterations
------------------------------------------------------------------
RMSD/seq/1048576              4.09 ms         4.08 ms          174
RMSD/seq/2097152              9.45 ms         9.43 ms           81
RMSD/seq/4194304              20.0 ms         19.9 ms           38
RMSD/seq/8388608              37.7 ms         37.6 ms           19
RMSD/seq/16777216             74.0 ms         73.8 ms            7
RMSD/par/1048576             0.281 ms        0.280 ms         2502
RMSD/par/2097152             0.946 ms        0.943 ms          732
RMSD/par/4194304              2.30 ms         2.30 ms          292
RMSD/par/8388608              5.12 ms         5.11 ms          122
RMSD/par/16777216             11.3 ms         11.2 ms           52
RMSD_TBBSA/seq/1048576        3.73 ms         3.72 ms          188
RMSD_TBBSA/seq/2097152        7.58 ms         7.56 ms           92
RMSD_TBBSA/seq/4194304        15.4 ms         15.4 ms           46
RMSD_TBBSA/seq/8388608        30.7 ms         30.6 ms           23
RMSD_TBBSA/seq/16777216       61.1 ms         61.0 ms           11
RMSD_TBBSA/par/1048576       0.155 ms        0.155 ms         4552
RMSD_TBBSA/par/2097152       0.337 ms        0.336 ms         2077
RMSD_TBBSA/par/4194304       0.958 ms        0.955 ms          751
RMSD_TBBSA/par/8388608        2.31 ms         2.31 ms          292
RMSD_TBBSA/par/16777216       5.13 ms         5.12 ms          125
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
