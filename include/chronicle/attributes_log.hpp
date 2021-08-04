/* This file is part of chronicle library
 * Copyright 2020 Andrei Ilin <ortfero@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#pragma once


#ifdef CHRONICLE_USE_SYSTEM_UFORMAT

#include <uformat/texter.hpp>

#else

#include "bundled/uformat/texter.hpp"

#endif // CHRONICLE_USE_SYSTEM_UFORMAT


#include "traits.hpp"
#include "data_log.hpp"


namespace chronicle {
  
  
template<typename Tr>
struct basic_attributes_log : basic_data_log<Tr> {

  using base = basic_data_log<Tr>;
  using size_type = typename base::size_type;
  using message_type = typename base::message_type;
  using buffer_type = typename base::data_type;
  
  static constexpr size_type default_message_size = 512;
  
  using base::open;
  using base::opened;
  using base::close;
  using base::severity;  
  
  
  basic_attributes_log(size_type message_size = default_message_size) noexcept:
    base(message_size)
  { }
  
  
  basic_attributes_log(basic_attributes_log const&) = delete;
  basic_attributes_log& operator = (basic_attributes_log const&) = delete;


  template<size_t N1, size_t N2>
  void failure(char const (&tag)[N1], char const (&text)[N2]) {
    base::print<severity::failure>(std::string_view{tag, N1 - 1},
                            std::string_view{text, N2 - 1});
  }


  template<size_t N1, size_t N2, size_t N3, typename Arg, typename... Attrs>
  void failure(char const (&tag)[N1], char const (&text)[N2],
               char const (&name)[N3], Arg&& value,
               Attrs&&... attrs) {
    print<severity::failure>(std::string_view{tag, N1 - 1},
                            std::string_view{text, N2 - 1},
                            std::string_view{name, N3 - 1},
                            std::forward<Arg>(value),
                            std::forward<Attrs>(attrs)...);
  }
  
  
  template<typename R, size_t N1, size_t N2>
  R failure_with(R&& r, char const (&tag)[N1], char const (&text)[N2]) {
    base::print<severity::failure>(std::string_view{tag, N1 - 1},
                            std::string_view{text, N2 - 1});
    return std::forward<R>(r);
  }


  template<typename R, size_t N1, size_t N2, size_t N3, typename Arg, typename... Attrs>
  R failure_with(R&& r, char const (&tag)[N1], char const (&text)[N2],
               char const (&name)[N3], Arg&& value,
               Attrs&&... attrs) {
    print<severity::failure>(std::string_view{tag, N1 - 1},
                            std::string_view{text, N2 - 1},
                            std::string_view{name, N3 - 1},
                            std::forward<Arg>(value),
                            std::forward<Attrs>(attrs)...);
    return std::forward<R>(r);
  }



  template<size_t N1, size_t N2>
  void error(char const (&tag)[N1], char const (&text)[N2]) {
    base::print<severity::error>(std::string_view{tag, N1 - 1},
                            std::string_view{text, N2 - 1});
  }


  template<size_t N1, size_t N2, size_t N3, typename Arg, typename... Attrs>
  void error(char const (&tag)[N1], char const (&text)[N2],
               char const (&name)[N3], Arg&& value,
               Attrs&&... attrs) {
    print<severity::error>(std::string_view{tag, N1 - 1},
                            std::string_view{text, N2 - 1},
                            std::string_view{name, N3 - 1},
                            std::forward<Arg>(value),
                            std::forward<Attrs>(attrs)...);
  }


  template<typename R, size_t N1, size_t N2>
  R error_with(R&& r, char const (&tag)[N1], char const (&text)[N2]) {
    base::print<severity::error>(std::string_view{tag, N1 - 1},
                            std::string_view{text, N2 - 1});
    return std::forward<R>(r);
  }


  template<typename R, size_t N1, size_t N2, size_t N3, typename Arg, typename... Attrs>
  R error_with(R&& r, char const (&tag)[N1], char const (&text)[N2],
               char const (&name)[N3], Arg&& value,
               Attrs&&... attrs) {
    print<severity::error>(std::string_view{tag, N1 - 1},
                            std::string_view{text, N2 - 1},
                            std::string_view{name, N3 - 1},
                            std::forward<Arg>(value),
                            std::forward<Attrs>(attrs)...);
    return std::forward<R>(r);
  }



  template<size_t N1, size_t N2>
  void warning(char const (&tag)[N1], char const (&text)[N2]) {
    base::print<severity::warning>(std::string_view{tag, N1 - 1},
                            std::string_view{text, N2 - 1});
  }


  template<size_t N1, size_t N2, size_t N3, typename Arg, typename... Attrs>
  void warning(char const (&tag)[N1], char const (&text)[N2],
               char const (&name)[N3], Arg&& value,
               Attrs&&... attrs) {
    print<severity::warning>(std::string_view{tag, N1 - 1},
                            std::string_view{text, N2 - 1},
                            std::string_view{name, N3 - 1},
                            std::forward<Arg>(value),
                            std::forward<Attrs>(attrs)...);
  }
  
  
  template<typename R, size_t N1, size_t N2>
  R warning_with(R&& r, char const (&tag)[N1], char const (&text)[N2]) {
    base::print<severity::warning>(std::string_view{tag, N1 - 1},
                            std::string_view{text, N2 - 1});
    return std::forward<R>(r);
  }


  template<typename R, size_t N1, size_t N2, size_t N3, typename Arg, typename... Attrs>
  R warning_with(R&& r, char const (&tag)[N1], char const (&text)[N2],
               char const (&name)[N3], Arg&& value,
               Attrs&&... attrs) {
    print<severity::warning>(std::string_view{tag, N1 - 1},
                            std::string_view{text, N2 - 1},
                            std::string_view{name, N3 - 1},
                            std::forward<Arg>(value),
                            std::forward<Attrs>(attrs)...);
    return std::forward<R>(r);
  }



  template<size_t N1, size_t N2>
  void info(char const (&tag)[N1], char const (&text)[N2]) {
    base::print<severity::info>(std::string_view{tag, N1 - 1},
                            std::string_view{text, N2 - 1});
  }


  template<size_t N1, size_t N2, size_t N3, typename Arg, typename... Attrs>
  void info(char const (&tag)[N1], char const (&text)[N2],
               char const (&name)[N3], Arg&& value,
               Attrs&&... attrs) {
    print<severity::info>(std::string_view{tag, N1 - 1},
                            std::string_view{text, N2 - 1},
                            std::string_view{name, N3 - 1},
                            std::forward<Arg>(value),
                            std::forward<Attrs>(attrs)...);
  }
  
  
  template<typename R, size_t N1, size_t N2>
  R info_with(R&& r, char const (&tag)[N1], char const (&text)[N2]) {
    base::print<severity::info>(std::string_view{tag, N1 - 1},
                            std::string_view{text, N2 - 1});
    return std::forward<R>(r);
  }


  template<typename R, size_t N1, size_t N2, size_t N3, typename Arg, typename... Attrs>
  R info_with(R&& r, char const (&tag)[N1], char const (&text)[N2],
               char const (&name)[N3], Arg&& value,
               Attrs&&... attrs) {
    print<severity::info>(std::string_view{tag, N1 - 1},
                            std::string_view{text, N2 - 1},
                            std::string_view{name, N3 - 1},
                            std::forward<Arg>(value),
                            std::forward<Attrs>(attrs)...);
    return std::forward<R>(r);
  }



  template<size_t N1, size_t N2>
  void extra(char const (&tag)[N1], char const (&text)[N2]) {
    base::print<severity::extra>(std::string_view{tag, N1 - 1},
                            std::string_view{text, N2 - 1});
  }


  template<size_t N1, size_t N2, size_t N3, typename Arg, typename... Attrs>
  void extra(char const (&tag)[N1], char const (&text)[N2],
               char const (&name)[N3], Arg&& value,
               Attrs&&... attrs) {
    print<severity::extra>(std::string_view{tag, N1 - 1},
                            std::string_view{text, N2 - 1},
                            std::string_view{name, N3 - 1},
                            std::forward<Arg>(value),
                            std::forward<Attrs>(attrs)...);
  }
  
  
  template<typename R, size_t N1, size_t N2>
  R extra_with(R&& r, char const (&tag)[N1], char const (&text)[N2]) {
    base::print<severity::extra>(std::string_view{tag, N1 - 1},
                            std::string_view{text, N2 - 1});
    return std::forward<R>(r);
  }


  template<typename R, size_t N1, size_t N2, size_t N3, typename Arg, typename... Attrs>
  R extra_with(R&& r, char const (&tag)[N1], char const (&text)[N2],
               char const (&name)[N3], Arg&& value,
               Attrs&&... attrs) {
    print<severity::extra>(std::string_view{tag, N1 - 1},
                            std::string_view{text, N2 - 1},
                            std::string_view{name, N3 - 1},
                            std::forward<Arg>(value),
                            std::forward<Attrs>(attrs)...);
    return std::forward<R>(r);
  }



  template<size_t N1, size_t N2>
  void trace(char const (&tag)[N1], char const (&text)[N2]) {
    base::print<severity::trace>(std::string_view{tag, N1 - 1},
                            std::string_view{text, N2 - 1});
  }


  template<size_t N1, size_t N2, size_t N3, typename Arg, typename... Attrs>
  void trace(char const (&tag)[N1], char const (&text)[N2],
               char const (&name)[N3], Arg&& value,
               Attrs&&... attrs) {
    print<severity::trace>(std::string_view{tag, N1 - 1},
                            std::string_view{text, N2 - 1},
                            std::string_view{name, N3 - 1},
                            std::forward<Arg>(value),
                            std::forward<Attrs>(attrs)...);
  }
  
  
  template<typename R, size_t N1, size_t N2>
  R trace_with(R&& r, char const (&tag)[N1], char const (&text)[N2]) {
    base::print<severity::trace>(std::string_view{tag, N1 - 1},
                            std::string_view{text, N2 - 1});
    return std::forward<R>(r);
  }


  template<typename R, size_t N1, size_t N2, size_t N3, typename Arg, typename... Attrs>
  R trace_with(R&& r, char const (&tag)[N1], char const (&text)[N2],
               char const (&name)[N3], Arg&& value,
               Attrs&&... attrs) {
    print<severity::trace>(std::string_view{tag, N1 - 1},
                            std::string_view{text, N2 - 1},
                            std::string_view{name, N3 - 1},
                            std::forward<Arg>(value),
                            std::forward<Attrs>(attrs)...);
    return std::forward<R>(r);
  }



#ifdef NDEBUG


  template<size_t N1, size_t N2>
  void debug(char const (&)[N1], char const (&)[N2])
  { }


  template<size_t N1, size_t N2, size_t N3, typename Arg, typename... Attrs>
  void debug(char const (&)[N1], char const (&)[N2],
               char const (&)[N3], Arg&&,
               Attrs&&...)
  { }
  
  
  template<typename R, size_t N1, size_t N2>
  R debug_with(R&& r, char const (&)[N1], char const (&)[N2]) {
    return std::forward<R>(r);
  }


  template<typename R, size_t N1, size_t N2, size_t N3, typename Arg, typename... Attrs>
  R debug_with(R&& r, char const (&)[N1], char const (&)[N2],
               char const (&)[N3], Arg&&, Attrs&&...) {
    return std::forward<R>(r);
  }



#else


  template<size_t N1, size_t N2>
  void debug(char const (&tag)[N1], char const (&text)[N2]) {
    base::print<severity::debug>(std::string_view{tag, N1 - 1},
                            std::string_view{text, N2 - 1});
  }


  template<size_t N1, size_t N2, size_t N3, typename Arg, typename... Attrs>
  void debug(char const (&tag)[N1], char const (&text)[N2],
               char const (&name)[N3], Arg&& value,
               Attrs&&... attrs) {
    print<severity::debug>(std::string_view{tag, N1 - 1},
                            std::string_view{text, N2 - 1},
                            std::string_view{name, N3 - 1},
                            std::forward<Arg>(value),
                            std::forward<Attrs>(attrs)...);
  }
  
  
  template<typename R, size_t N1, size_t N2>
  R debug_with(R&& r, char const (&tag)[N1], char const (&text)[N2]) {
    base::print<severity::error>(std::string_view{tag, N1 - 1},
                            std::string_view{text, N2 - 1});
    return std::forward<R>(r);
  }


  template<typename R, size_t N1, size_t N2, size_t N3, typename Arg, typename... Attrs>
  R debug_with(R&& r, char const (&tag)[N1], char const (&text)[N2],
               char const (&name)[N3], Arg&& value,
               Attrs&&... attrs) {
    print<severity::error>(std::string_view{tag, N1 - 1},
                            std::string_view{text, N2 - 1},
                            std::string_view{name, N3 - 1},
                            std::forward<Arg>(value),
                            std::forward<Attrs>(attrs)...);
    return std::forward<R>(r);
  }



#endif


private:

  template<chronicle::severity S, typename Arg, typename... Attrs>
  void print(std::string_view const& tag, std::string_view const& text,
             std::string_view const& name, Arg&& value,
             Attrs&&... attrs) {
    if(base::severity() < S)
      return;
    message_type* m = base::template claim<S>(tag, text);
    if(!m)
      return;
    m->data.clear();
    m->data << ' ';
    m->data.attributes(name, std::forward<Arg>(value), std::forward<Attrs>(attrs)...);
    m->has_data = true;
    base::publish(*m);
  }

}; // basic_attributes_log


using attributes_log_single_traits = traits_default_single<uformat::long_texter>;
using attributes_log_traits = traits_default<uformat::long_texter>;


using attributes_log_single = basic_attributes_log<attributes_log_single_traits>;
using attributes_log = basic_attributes_log<attributes_log_traits>;

  
} // chronicle
