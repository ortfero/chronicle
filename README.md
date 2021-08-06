# chronicle

C++17 header-only library for low-latency asynchronous logging

## Usage

Just put directory `include/chronicle` at your include path

## Tests

To build tests:

```shell
cd chronicle
mkdir build
cd build
meson ../test
ninja
```

## Benchmark

Logging string, integer number and float number `100000` times.

| Logger                                                | Version | Date       | Latency (ns) | Ratio |
|-------------------------------------------------------|--------:|-----------:|-------------:|------:|
| chronicle                                             |         | 06.08.2021 | 272          | x2.9  |
| [NanoLog](https://github.com/Iyengar111/NanoLog)      |         | 08.03.2017 | 283          | x3.0  |
| [reckless](https://github.com/mattiasflodin/reckless) | 3.0.3   | 10.04.2021 | 95           | x1    |
| [spdlog](https://github.com/gabime/spdlog)            | 1.9.1   | 27.07.2021 | 2529         | x26.6 |


## Snippets

### Opening single-threaded text logger

```cpp
#include <system_error>
#include <chronicle/text_log.hpp>
#include <chronicle/sinks/file.hpp>

namespace cr = chronicle;
cr::unique_text_log log; // 'cr::shared_text_log' for multi-threaded

int main() {
  
  std::error_code failed; auto file = cr::sinks::file::open("test.log", failed);
  if(!file)
    return 1;
  log.add_sink(std::move(file));
  if(!log.open()
    return 1;
  
  // Should be: "source", "message", {parameter} 
  log.info("main", "Some message", "{name1: ", 127, ", name2: ", "'value2'}");
  
  // Possible output:
  //         2020-05-09 16:57:02.343402 [main] Some message {name1: 127, name2: 'value2'}
  
  return 0;
}
```

### Opening multithreaded structured logger with daily rotation

```cpp
#include <chronicle/structured_log.hpp>
#include <chronicle/sinks/daily_rotated_file.hpp>

namespace cr = chronicle;
cr::unique_structured_log log;

int main() {
  
  // Something like "test-2020_05_09.log" will be opened
  std::error_code failed; auto file = cr::sinks::daily_rotated_file::open("test.log", failed);
  if(!file)
    return 1;
  log.add_sink(std::move(file));
  if(!log.open())
    return 1;
  
  // Should be: "source", "message", {"name", value} 
  log.info("main", "Some message", "name1", 127, "name2", "value2");
  
  // Possible output:
  //         2020-05-09 16:57:02.343402 [14648] [main] Some message {name1: 127, name2: "value2"}
  
  return 0;
}
```

### Using milliseconds instead of microseconds for single-threaded log

```cpp
#include <chronicle/structured_log.hpp>
#include <chronicle/traits.hpp>

namespace cr = chronicle;

using unique_log = cr::basic_structured_log<cr::traits_unique_ms>;
```


### Using only time (without date) for multithreaded log

```cpp
#include <chronicle/structured_log.hpp>
#include <chronicle/traits.hpp>

namespace cr = chronicle;

using shared_log = cr::basic_structured_log<cr::traits_shared_time_only_us>;
```


### Logging custom type

```cpp
struct point {
  int x, y;
};

template<typename S> S&
operator << (S& stream, point const& p) {
  return s << "{x:" << p.x << ", y:" << p.y << '}';
}
```


## Configuration macros

* ```CHRONICLE_USE_SYSTEM_DATE``` - use system date library instead of bundled
* ```CHRONICLE_USE_SYSTEM_UFORMAT``` - use system uformat library instead of bundled
* ```CHRONICLE_USE_SYSTEM_HYDRA``` - use system hydra library instead of bundled
