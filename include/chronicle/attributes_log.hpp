#pragma once


#include "detail/hydra/mpsc_queue.hpp"
#include "detail/hydra/spsc_queue.hpp"
#include "detail/chineseroom/texter.hpp"
#include "data_log.hpp"
#include "sinks/output.hpp"
#include "sinks/file.hpp"


namespace chronicle {
  
  
template<typename S, typename B, typename Q = hydra::mpsc_queue<message<B>>>
struct basic_attributes_log : basic_data_log<S, B, Q> {

  using base = basic_data_log<S, B, Q>;
  using buffer_type = B;
  using size_type = typename base::size_type;
  
  static constexpr size_type default_buffer_for_message = 512;
  
  using base::open;
  using base::close;
  using base::reserve;
  
  
  basic_attributes_log(size_type buffer_for_message = default_buffer_for_message) noexcept:
    base(buffer_for_message)
  { }
  
  
  basic_attributes_log(basic_attributes_log const&) = delete;
  basic_attributes_log& operator = (basic_attributes_log const&) = delete;


  template<size_t N1, size_t N2> void notice(char const (&tag)[N1], char const (&text)[N2]) {
    base::print<severity::notice>(std::string_view{tag, N1 - 1},
                                  std::string_view{text, N2 - 1});
  }


  template<size_t N1, size_t N2, size_t N3, typename Arg, typename... Args>
  void notice(char const (&tag)[N1], char const (&text)[N2],
              char const (&attr_name)[N3], Arg&& attr_value,
              Args&&... attrs) {
    print<severity::notice>(std::string_view{tag, N1 - 1},
                            std::string_view{text, N2 - 1},
                            std::string_view{attr_name, N3 - 1},
                            std::forward<Arg>(attr_value),
                            std::forward<Args>(attrs)...);
  }


private:

  template<severity S, typename Arg, typename... Args>
  void print(std::string_view const& tag, std::string_view const& text,
             std::string_view const& attr_name, Arg&& attr_value,
             Args&&... attrs) {
    auto& m = base::lease<S>(tag, text);
    m.data.clear();
    m.data << ' ' << '{';
    m.data << attr_name << ' ' << '=' << ' ' << attr_value;
    format_attributes(m.data, std::forward<Args>(attrs)...);
    m.data << '}';
    m.has_data = true;
    base::enqueue(std::move(m));
  }


  static void format_attributes(buffer&)
  { }


  template<typename S, size_t N, typename T, typename... Args>
  static void format_attributes(printer<S>& buffer, char const (&attr_name)[N],
                                T&& attr_value, Args&&... args) {
    buffer << ',' << ' ' << std::string_view{attr_name, N - 1} << ' ' << '=' << ' ' << attr_value;
    format_attributes(buffer, std::forward<Args>(args)...);
  }


};


template<typename S, typename B = chineseroom::long_texter>
using attributes_log_mt = basic_attributes_log<S, B>;

template<typename S, typename B = chineseroom::long_texter>
using attributes_log = basic_attributes_log<S, B, hydra::spsc_queue<message<B>>>;

template<typename B = chineseroom::long_texter>
using output_attributes_log_mt = attributes_log_mt<sinks::output, B>;

template<typename B = chineseroom::long_texter>
using output_attributes_log = attributes_log<sinks::output, B>;

template<typename B = chineseroom::long_texter>
using file_attributes_log_mt = attributes_log_mt<sinks::file, B>;

template<typename B = chineseroom::long_texter>
using file_attributes_log = attributes_log<sinks::file, B>;
  
  
} // chronicle
