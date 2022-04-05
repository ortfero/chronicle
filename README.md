# chronicle

C++17 header-only library for low-latency asynchronous logging


## Usage

Just put directory `include/chronicle` at your include path


## Tests and benchmark

To build tests and benchmark:

```shell
cd chronicle
mkdir build
cd build
meson ..
ninja
```


## Benchmark

Logging string, integer number and float number `100000` times.

| Logger                                                | Version | Date       | Latency (ns) | Ratio |
|-------------------------------------------------------|--------:|-----------:|-------------:|------:|
| chronicle                                             |         | 28.03.2022 | 1381         | x1.2  |
| [NanoLog](https://github.com/Iyengar111/NanoLog)      |         | 08.03.2017 | 2470         | x2.2  |
| [reckless](https://github.com/mattiasflodin/reckless) | 3.0.3   | 10.04.2021 | 1117         | x1    |
| [spdlog](https://github.com/gabime/spdlog)            | 1.10.0  | 13.08.2021 | 6324         | x5.7  |


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
  log.info("main", "Some message", "{ name1: ", 127, ", name2: ", "\"value2\" }");

  // Possible output:
  //         2020-05-09 16:57:02.343402 [main] Some message { name1: 127, name2: "value2" }

  return 0;
}
```

### Opening multithreaded structured logger with daily rotation

```cpp
#include <chronicle/structured_log.hpp>
#include <chronicle/sinks/daily_rotated_file.hpp>

namespace cr = chronicle;
cr::shared_structured_log log;

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
  //         2020-05-09 16:57:02.343402 [14648] [main] Some message { name1: 127, name2: "value2" }

  return 0;
}
```

### Using milliseconds instead of microseconds for single-threaded log

```cpp
#include <chronicle/structured_log.hpp>
#include <chronicle/traits.hpp>
#include <ufmt/text.hpp>

namespace cr = chronicle;

using unique_log = cr::structured_log<cr::traits_unique_ms<ufmt::text>>;
```


### Using only time (without date) for multithreaded log

```cpp
#include <chronicle/structured_log.hpp>
#include <chronicle/traits.hpp>
#include <ufmt/text.hpp>

namespace cr = chronicle;

using shared_log = cr::structured_log<cr::traits_shared_time_only_us<ufmt::text>>;
```


### Logging custom type

```cpp
struct point {
  int x, y;
};

template<typename S> S&
operator << (S& stream, point const& p) {
  return s << "{ x: " << p.x << ", y: " << p.y << " }";
}
```


### Using custom clock

```cpp
namespace cr = chronicle;

using unique_log = cr::structured_log<cr::traits_unique_ms<ufmt::text>,
                                      some_custom_clock>;
```

## Dependencies

* [date](https://github.com/HowardHinnant/date)
* [hydra](https://github.com/ortfero/hydra)
* [ufmt](https://github.com/ortfero/ufmt)
