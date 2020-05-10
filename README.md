# chronicle
Low-latency asynchronous logger

## Snippets

### Opening single-threaded text logger

```cpp
#include <system_error>
#include <chronicle/text_log.hpp>
#include <chronicle/sinks/file.hpp>

namespace cr = chronicle;
cr::text_log_single log; // 'cr::text_log' for multi-threaded

int main() {
  
  std::error_code failed; auto file = cr::sinks::file::open("test.log", failed);
  if(!file)
    return 1;
  log.add_sink(std::move(file));
  if(!log.open()
    return 1;
  
  // Should be: "source", "message", {parameter} 
  log.info("main", "Some message: ", "name1 = ", 127, ", name2 =", "'value2'");
  
  // Possible output:
  //         2020-05-09 16:57:02.343402 [main] Some message: name1 = 127, name2 = 'value2'
  
  return 0;
}
```

### Opening multi-threaded attributes logger with daily rotation

```cpp
#include <chronicle/attributes_log.hpp>
#include <chronicle/sinks/daily_rotated_file.hpp>

namespace cr = chronicle;
cr::attributes_log log;

int main() {
  
  // Something like "test-2020_05_09.log" will be opened
  std::error_code failed; auto file = cr::sinks::daily_rotated_file::open("test.log", failed);
  if(!file)
    return 1;
  log.add_sink(file);
  if(!log.open())
    return 1;
  
  // Should be: "source", "message", {"name", value} 
  log.info("main", "Some message", "name1", 127, "name2", "value2");
  
  // Possible output:
  //         2020-05-09 16:57:02.343402 [14648] [main] Some message {name1 = 127, name2 = "value2"}
  
  return 0;
}
```

### Using milliseconds instead of microseconds

```cpp
#include <chronicle/attributes_log.hpp>
#include <chronicle/traits.hpp>

namespace cr = chronicle;
struct own_log_traits : cr::attributes_log_traits {
  using format_type = cr::fields::format_milliseconds;
};

using own_log = cr::basic_attributes_log<own_log_traits>;
```

### Logging custom type

```cpp
#include <chronicle/bundled/chineseroom/texter.hpp>

struct point {
  int x, y;
};

template<typename S> chineseroom::texter<S>&
operator << (chineseroom::texter<S>& texter, point const& p) {
  return texter.print("{x = ", p.x, ", y = ", p.y, '}');
  // or texter.attributes("x", p.x, "y", p.y);
}

```


## Configuration macros

* ```CHRONICLE_USE_SYSTEM_DATE``` - use system date library instead of bundled
* ```CHRONICLE_USE_SYSTEM_UFORMAT``` - use system uformat library instead of bundled
* ```CHRONICLE_USE_SYSTEM_THEATER``` - use system theater library instead of bundled
