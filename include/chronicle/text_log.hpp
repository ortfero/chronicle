#pragma once


#include "detail/chineseroom/texter.hpp"
#include "detail/hydra/spsc_queue.hpp"
#include "detail/hydra/mpsc_queue.hpp"
#include "data_log.hpp"
#include "sinks/output.hpp"
#include "sinks/file.hpp"


namespace chronicle {
  

template<typename S, typename B, typename Q = hydra::mpsc_queue<message<B>>>
struct basic_text_log : basic_data_log<S, B, Q> {  

  using base = basic_data_log<S, B, Q>;
  using buffer_type = B;
  using size_type = typename base::size_type;
  
  static constexpr size_type default_buffer_for_message = 512;

  using base::open;
  using base::close;
  
  basic_text_log(size_type buffer_for_message = default_buffer_for_message) noexcept:
    base(buffer_for_message)
  { }
  
  
  basic_text_log(basic_text_log const&) = delete;
  basic_text_log& operator = (basic_text_log const&) = delete;
  

  template<size_t N1, size_t N2>
  void notice(char const (&tag)[N1], char const (&text)[N2]) {
    base::print<severity::notice>(std::string_view{tag, N1 - 1},
                                  std::string_view{text, N2 - 1});
  }


  template<size_t N1, size_t N2, typename... Args>
  void notice(char const (&tag)[N1], char const (&text)[N2], Args&&... args) {
    print<severity::notice>(std::string_view{tag, N1 - 1},
                            std::string_view{text, N2 - 1},
                            std::forward<Args>(args)...);
  }


private:

  template<severity S, typename... Args>
  void print(std::string_view const& tag, std::string_view const& text, Args&&... args) {
    message<B>* m = base::claim<S>(tag, text);
    if(!m)
      return;
    m->data.clear();
    format_args(m->data, std::forward<Args>(args)...);
    m->has_data = true;
    base::publish(*m);
  }


  static void format_args(buffer_type&)
  { }


  template<typename T, typename... Args>
  static void format_args(buffer_type& p, T&& arg, Args&&... args) {
    p << arg;
    format_args(p, std::forward<Args>(args)...);
  }
};  // basic_text_log


template<typename S, typename B = chineseroom::long_texter>
using text_log_mt = basic_text_log<S, B>;

template<typename S, typename B = chineseroom::long_texter>
using text_log = basic_text_log<S, B, hydra::spsc_queue<message<B>>>;

template<typename B = chineseroom::long_texter>
using output_text_log_mt = text_log_mt<sinks::output, B>;

template<typename B = chineseroom::long_texter>
using output_text_log = text_log<sinks::output, B>;

template<typename B = chineseroom::long_texter>
using file_text_log_mt = text_log_mt<sinks::file, B>;

template<typename B = chineseroom::long_texter>
using file_text_log = text_log<sinks::file, B>;

  
} // chronicle
