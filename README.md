# chronicle

C++20 header-only library for low-latency asynchronous logging


## Usage

Just place contents of `include` directory at your include path


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
| chronicle                                             |         | 03.05.2022 | 1467         | x1.26 |
| [NanoLog](https://github.com/Iyengar111/NanoLog)      |         | 08.03.2017 | 2583         | x2.22 |
| [reckless](https://github.com/mattiasflodin/reckless) | 3.0.3   | 10.04.2021 | 1164         | x1    |
| [spdlog](https://github.com/gabime/spdlog)            | 1.10.0  | 04.04.2022 | 8345         | x7.17 |


## Snippets

### Opening single-threaded text logger

```cpp
#include <iostream>
#include <system_error>
#include <chronicle/text_log.hpp>
#include <chronicle/sinks/file.hpp>

namespace cr = chronicle;
cr::unique_text_log log; // 'cr::shared_text_log' for multi-threaded

int main() {

    auto const opened = log.open(cr::sinks::file::open("test.log"));
    if(!opened) {
        std::cerr << opened.error().message() << '\n';
        return -1;
    }

    // Should be: "source", "message", {parameter}
    log.info("main", "Some message", "{ name1: ", 127, ", name2: ", "\"value2\" }");

    // Possible output:
    //         2020-05-09 16:57:02.343402 [main] Some message { name1: 127, name2: "value2" }

    return 0;
}
```

### Opening multithreaded structured logger with daily rotation

```cpp
#include <iostream>
#include <chronicle/structured_log.hpp>
#include <chronicle/sinks/daily_rotated_file.hpp>

namespace cr = chronicle;
cr::shared_structured_log log;

int main() {

    // Something like "test-2020_05_09.log" will be opened
    auto const opened = log.open(cr::sinks::daily_rotated_file::open("test.log", failed));
    if(!opened) {
        std::cerr << opened.error().message() << '\n';
        return -1;
    }

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

## Dependencies

* [etceteras](https://github.com/ortfero/etceteras)
* [hydra](https://github.com/ortfero/hydra)
* [ufmt](https://github.com/ortfero/ufmt)
