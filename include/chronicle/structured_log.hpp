// This file is part of chronicle library
// Copyright 2020-2022 Andrei Ilin <ortfero@gmail.com>
// SPDX-License-Identifier: MIT

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
struct structured_log : data_log<Tr> {

  using base = data_log<Tr>;
  using size_type = typename base::size_type;
  using message_type = typename base::message_type;
  using buffer_type = typename base::data_type;
  
  static constexpr size_type default_message_size = 512;
  
  using base::open;
  using base::opened;
  using base::close;
  using base::severity;  
  
  
  structured_log(size_type message_size = default_message_size) noexcept:
    base(message_size)
  { }
  
  
  structured_log(structured_log const&) = delete;
  structured_log& operator=(structured_log const&) = delete;


  template<size_t N1, size_t N2>
  void failure(char const (&tag)[N1], char const (&text)[N2]) {
    base::template print<severity::failure>(std::string_view{tag, N1 - 1},
                            std::string_view{text, N2 - 1});
  }


  template<size_t N1, size_t N2, size_t N3, typename Arg, typename... Attrs>
  void failure(char const (&tag)[N1], char const (&text)[N2],
               char const (&name)[N3], Arg&& value,
               Attrs&&... attrs) {
    this->template print<severity::failure>(std::string_view{tag, N1 - 1},
                            std::string_view{text, N2 - 1},
                            std::string_view{name, N3 - 1},
                            std::forward<Arg>(value),
                            std::forward<Attrs>(attrs)...);
  }
  
  
  template<typename R, size_t N1, size_t N2>
  R failure_with(R&& r, char const (&tag)[N1], char const (&text)[N2]) {
    base::template print<severity::failure>(std::string_view{tag, N1 - 1},
                            std::string_view{text, N2 - 1});
    return std::forward<R>(r);
  }


  template<typename R, size_t N1, size_t N2, size_t N3, typename Arg, typename... Attrs>
  R failure_with(R&& r, char const (&tag)[N1], char const (&text)[N2],
               char const (&name)[N3], Arg&& value,
               Attrs&&... attrs) {
    this->template print<severity::failure>(std::string_view{tag, N1 - 1},
                            std::string_view{text, N2 - 1},
                            std::string_view{name, N3 - 1},
                            std::forward<Arg>(value),
                            std::forward<Attrs>(attrs)...);
    return std::forward<R>(r);
  }



  template<size_t N1, size_t N2>
  void error(char const (&tag)[N1], char const (&text)[N2]) {
    base::template print<severity::error>(std::string_view{tag, N1 - 1},
                            std::string_view{text, N2 - 1});
  }


  template<size_t N1, size_t N2, size_t N3, typename Arg, typename... Attrs>
  void error(char const (&tag)[N1], char const (&text)[N2],
               char const (&name)[N3], Arg&& value,
               Attrs&&... attrs) {
    this->template print<severity::error>(std::string_view{tag, N1 - 1},
                            std::string_view{text, N2 - 1},
                            std::string_view{name, N3 - 1},
                            std::forward<Arg>(value),
                            std::forward<Attrs>(attrs)...);
  }


  template<typename R, size_t N1, size_t N2>
  R error_with(R&& r, char const (&tag)[N1], char const (&text)[N2]) {
    base::template print<severity::error>(std::string_view{tag, N1 - 1},
                            std::string_view{text, N2 - 1});
    return std::forward<R>(r);
  }


  template<typename R, size_t N1, size_t N2, size_t N3, typename Arg, typename... Attrs>
  R error_with(R&& r, char const (&tag)[N1], char const (&text)[N2],
               char const (&name)[N3], Arg&& value,
               Attrs&&... attrs) {
    this->template print<severity::error>(std::string_view{tag, N1 - 1},
                            std::string_view{text, N2 - 1},
                            std::string_view{name, N3 - 1},
                            std::forward<Arg>(value),
                            std::forward<Attrs>(attrs)...);
    return std::forward<R>(r);
  }



  template<size_t N1, size_t N2>
  void warning(char const (&tag)[N1], char const (&text)[N2]) {
    base::template print<severity::warning>(std::string_view{tag, N1 - 1},
                            std::string_view{text, N2 - 1});
  }


  template<size_t N1, size_t N2, size_t N3, typename Arg, typename... Attrs>
  void warning(char const (&tag)[N1], char const (&text)[N2],
               char const (&name)[N3], Arg&& value,
               Attrs&&... attrs) {
    this->template print<severity::warning>(std::string_view{tag, N1 - 1},
                            std::string_view{text, N2 - 1},
                            std::string_view{name, N3 - 1},
                            std::forward<Arg>(value),
                            std::forward<Attrs>(attrs)...);
  }
  
  
  template<typename R, size_t N1, size_t N2>
  R warning_with(R&& r, char const (&tag)[N1], char const (&text)[N2]) {
    base::template print<severity::warning>(std::string_view{tag, N1 - 1},
                            std::string_view{text, N2 - 1});
    return std::forward<R>(r);
  }


  template<typename R, size_t N1, size_t N2, size_t N3, typename Arg, typename... Attrs>
  R warning_with(R&& r, char const (&tag)[N1], char const (&text)[N2],
               char const (&name)[N3], Arg&& value,
               Attrs&&... attrs) {
    this->template print<severity::warning>(std::string_view{tag, N1 - 1},
                            std::string_view{text, N2 - 1},
                            std::string_view{name, N3 - 1},
                            std::forward<Arg>(value),
                            std::forward<Attrs>(attrs)...);
    return std::forward<R>(r);
  }



  template<size_t N1, size_t N2>
  void info(char const (&tag)[N1], char const (&text)[N2]) {
    base::template print<severity::info>(std::string_view{tag, N1 - 1},
                            std::string_view{text, N2 - 1});
  }


  template<size_t N1, size_t N2, size_t N3, typename Arg, typename... Attrs>
  void info(char const (&tag)[N1], char const (&text)[N2],
               char const (&name)[N3], Arg&& value,
               Attrs&&... attrs) {
    this->template print<severity::info>(std::string_view{tag, N1 - 1},
                            std::string_view{text, N2 - 1},
                            std::string_view{name, N3 - 1},
                            std::forward<Arg>(value),
                            std::forward<Attrs>(attrs)...);
  }
  
  
  template<typename R, size_t N1, size_t N2>
  R info_with(R&& r, char const (&tag)[N1], char const (&text)[N2]) {
    base::template print<severity::info>(std::string_view{tag, N1 - 1},
                            std::string_view{text, N2 - 1});
    return std::forward<R>(r);
  }


  template<typename R, size_t N1, size_t N2, size_t N3, typename Arg, typename... Attrs>
  R info_with(R&& r, char const (&tag)[N1], char const (&text)[N2],
               char const (&name)[N3], Arg&& value,
               Attrs&&... attrs) {
    this->template print<severity::info>(std::string_view{tag, N1 - 1},
                            std::string_view{text, N2 - 1},
                            std::string_view{name, N3 - 1},
                            std::forward<Arg>(value),
                            std::forward<Attrs>(attrs)...);
    return std::forward<R>(r);
  }



  template<size_t N1, size_t N2>
  void extra(char const (&tag)[N1], char const (&text)[N2]) {
    base::template print<severity::extra>(std::string_view{tag, N1 - 1},
                            std::string_view{text, N2 - 1});
  }


  template<size_t N1, size_t N2, size_t N3, typename Arg, typename... Attrs>
  void extra(char const (&tag)[N1], char const (&text)[N2],
               char const (&name)[N3], Arg&& value,
               Attrs&&... attrs) {
    this->template print<severity::extra>(std::string_view{tag, N1 - 1},
                            std::string_view{text, N2 - 1},
                            std::string_view{name, N3 - 1},
                            std::forward<Arg>(value),
                            std::forward<Attrs>(attrs)...);
  }
  
  
  template<typename R, size_t N1, size_t N2>
  R extra_with(R&& r, char const (&tag)[N1], char const (&text)[N2]) {
    base::template print<severity::extra>(std::string_view{tag, N1 - 1},
                            std::string_view{text, N2 - 1});
    return std::forward<R>(r);
  }


  template<typename R, size_t N1, size_t N2, size_t N3, typename Arg, typename... Attrs>
  R extra_with(R&& r, char const (&tag)[N1], char const (&text)[N2],
               char const (&name)[N3], Arg&& value,
               Attrs&&... attrs) {
    this->template print<severity::extra>(std::string_view{tag, N1 - 1},
                            std::string_view{text, N2 - 1},
                            std::string_view{name, N3 - 1},
                            std::forward<Arg>(value),
                            std::forward<Attrs>(attrs)...);
    return std::forward<R>(r);
  }



  template<size_t N1, size_t N2>
  void trace(char const (&tag)[N1], char const (&text)[N2]) {
    base::template print<severity::trace>(std::string_view{tag, N1 - 1},
                            std::string_view{text, N2 - 1});
  }


  template<size_t N1, size_t N2, size_t N3, typename Arg, typename... Attrs>
  void trace(char const (&tag)[N1], char const (&text)[N2],
               char const (&name)[N3], Arg&& value,
               Attrs&&... attrs) {
    this->template print<severity::trace>(std::string_view{tag, N1 - 1},
                            std::string_view{text, N2 - 1},
                            std::string_view{name, N3 - 1},
                            std::forward<Arg>(value),
                            std::forward<Attrs>(attrs)...);
  }
  
  
  template<typename R, size_t N1, size_t N2>
  R trace_with(R&& r, char const (&tag)[N1], char const (&text)[N2]) {
    base::template print<severity::trace>(std::string_view{tag, N1 - 1},
                            std::string_view{text, N2 - 1});
    return std::forward<R>(r);
  }


  template<typename R, size_t N1, size_t N2, size_t N3, typename Arg, typename... Attrs>
  R trace_with(R&& r, char const (&tag)[N1], char const (&text)[N2],
               char const (&name)[N3], Arg&& value,
               Attrs&&... attrs) {
    this->template print<severity::trace>(std::string_view{tag, N1 - 1},
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
    base::template print<severity::debug>(std::string_view{tag, N1 - 1},
                            std::string_view{text, N2 - 1});
  }


  template<size_t N1, size_t N2, size_t N3, typename Arg, typename... Attrs>
  void debug(char const (&tag)[N1], char const (&text)[N2],
               char const (&name)[N3], Arg&& value,
               Attrs&&... attrs) {
    this->template print<severity::debug>(std::string_view{tag, N1 - 1},
                            std::string_view{text, N2 - 1},
                            std::string_view{name, N3 - 1},
                            std::forward<Arg>(value),
                            std::forward<Attrs>(attrs)...);
  }
  
  
  template<typename R, size_t N1, size_t N2>
  R debug_with(R&& r, char const (&tag)[N1], char const (&text)[N2]) {
    base::template print<severity::error>(std::string_view{tag, N1 - 1},
                            std::string_view{text, N2 - 1});
    return std::forward<R>(r);
  }


  template<typename R, size_t N1, size_t N2, size_t N3, typename Arg, typename... Attrs>
  R debug_with(R&& r, char const (&tag)[N1], char const (&text)[N2],
               char const (&name)[N3], Arg&& value,
               Attrs&&... attrs) {
    this->template print<severity::error>(std::string_view{tag, N1 - 1},
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

}; // structured_log


using unique_structured_log = structured_log<traits_unique_default<uformat::long_texter>>;
using shared_structured_log = structured_log<traits_shared_default<uformat::long_texter>>;

  
} // chronicle
