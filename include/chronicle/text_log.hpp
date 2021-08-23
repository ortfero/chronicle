/* This file is part of chronicle library
 * Copyright 2020-2021 Andrei Ilin <ortfero@gmail.com>
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
struct text_log : data_log<Tr> {

  using base = data_log<Tr>;
  using size_type = typename base::size_type;
  using message_type = typename base::message_type;
  using buffer_type = typename base::data_type;
  
  static constexpr size_type default_message_size = 512;

  using base::open;
  using base::opened;
  using base::close;
  using base::severity;
  
  text_log(size_type message_size = default_message_size) noexcept:
    base(message_size)
  { }
  
  
  text_log(text_log const&) = delete;
  text_log& operator=(text_log const&) = delete;
  

  template<size_t N1, size_t N2>
  void failure(char const (&tag)[N1], char const (&text)[N2]) {
    base::template print<severity::failure>(std::string_view{tag, N1 - 1},
                                  std::string_view{text, N2 - 1});
  }


  template<size_t N1, size_t N2, typename Arg, typename... Args>
  void failure(char const (&tag)[N1], char const (&text)[N2], Arg&& arg, Args&&... args) {
    this->template print<severity::failure>(std::string_view{tag, N1 - 1},
                            std::string_view{text, N2 - 1},
                            std::forward<Arg>(arg),
                            std::forward<Args>(args)...);
  }


  template<size_t N1, size_t N2>
  void error(char const (&tag)[N1], char const (&text)[N2]) {
    base::template print<severity::error>(std::string_view{tag, N1 - 1},
                                  std::string_view{text, N2 - 1});
  }


  template<size_t N1, size_t N2, typename Arg, typename... Args>
  void error(char const (&tag)[N1], char const (&text)[N2], Arg&& arg, Args&&... args) {
    this->template print<severity::error>(std::string_view{tag, N1 - 1},
                            std::string_view{text, N2 - 1},
                            std::forward<Arg>(arg),
                            std::forward<Args>(args)...);
  }


  template<size_t N1, size_t N2>
  void warning(char const (&tag)[N1], char const (&text)[N2]) {
    base::template print<severity::warning>(std::string_view{tag, N1 - 1},
                                  std::string_view{text, N2 - 1});
  }


  template<size_t N1, size_t N2, typename Arg, typename... Args>
  void warning(char const (&tag)[N1], char const (&text)[N2], Arg&& arg, Args&&... args) {
    this->template print<severity::warning>(std::string_view{tag, N1 - 1},
                            std::string_view{text, N2 - 1},
                            std::forward<Arg>(arg),
                            std::forward<Args>(args)...);
  }


  template<size_t N1, size_t N2>
  void info(char const (&tag)[N1], char const (&text)[N2]) {
    base::template print<severity::info>(std::string_view{tag, N1 - 1},
                                  std::string_view{text, N2 - 1});
  }


  template<size_t N1, size_t N2, typename Arg, typename... Args>
  void info(char const (&tag)[N1], char const (&text)[N2], Arg&& arg, Args&&... args) {
    this->template print<severity::info>(std::string_view{tag, N1 - 1},
                            std::string_view{text, N2 - 1},
                            std::forward<Arg>(arg),
                            std::forward<Args>(args)...);
  }


  template<size_t N1, size_t N2>
  void extra(char const (&tag)[N1], char const (&text)[N2]) {
    base::template print<severity::extra>(std::string_view{tag, N1 - 1},
                                  std::string_view{text, N2 - 1});
  }


  template<size_t N1, size_t N2, typename Arg, typename... Args>
  void extra(char const (&tag)[N1], char const (&text)[N2], Arg&& arg, Args&&... args) {
    this->template print<severity::extra>(std::string_view{tag, N1 - 1},
                            std::string_view{text, N2 - 1},
                            std::forward<Arg>(arg),
                            std::forward<Args>(args)...);
  }


  template<size_t N1, size_t N2>
  void trace(char const (&tag)[N1], char const (&text)[N2]) {
    base::template print<severity::trace>(std::string_view{tag, N1 - 1},
                                  std::string_view{text, N2 - 1});
  }


  template<size_t N1, size_t N2, typename Arg, typename... Args>
  void trace(char const (&tag)[N1], char const (&text)[N2], Arg&& arg, Args&&... args) {
    this->template print<severity::trace>(std::string_view{tag, N1 - 1},
                            std::string_view{text, N2 - 1},
                            std::forward<Arg>(arg),
                            std::forward<Args>(args)...);
  }

#ifdef NDEBUG


  template<size_t N1, size_t N2>
  void debug(char const (&)[N1], char const (&)[N2])
  { }


  template<size_t N1, size_t N2, typename Arg, typename... Args>
  void debug(char const (&)[N1], char const (&)[N2], Arg&&, Args&&...)
  { }


#else

  template<size_t N1, size_t N2>
  void debug(char const (&tag)[N1], char const (&text)[N2]) {
    base::template print<severity::debug>(std::string_view{tag, N1 - 1},
                                  std::string_view{text, N2 - 1});
  }


  template<size_t N1, size_t N2, typename Arg, typename... Args>
  void debug(char const (&tag)[N1], char const (&text)[N2], Arg&& arg, Args&&... args) {
    this->template print<severity::debug>(std::string_view{tag, N1 - 1},
                            std::string_view{text, N2 - 1},
                            std::forward<Arg>(arg),
                            std::forward<Args>(args)...);
  }

#endif




private:


  template<chronicle::severity S, typename Arg, typename... Args>
  void print(std::string_view const& tag, std::string_view const& text, Arg&& arg, Args&&... args) {
    if(base::severity() < S)
      return;
    message_type* m = base::template claim<S>(tag, text);
    if(!m)
      return;
    m->data.clear();
    format_args(m->data, std::forward<Arg>(arg), std::forward<Args>(args)...);
    m->has_data = true;
    base::publish(*m);
  }


  static void format_args(buffer_type&)
  { }


  template<typename Arg, typename... Args>
  static void format_args(buffer_type& p, Arg&& arg, Args&&... args) {
    p << arg;
    format_args(p, std::forward<Args>(args)...);
  }
};  // text_log


using unique_text_log = text_log<traits_unique_default<uformat::long_texter>>;
using shared_text_log = text_log<traits_shared_default<uformat::long_texter>>;

  
} // chronicle
