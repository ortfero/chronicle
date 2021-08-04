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


#include <thread>
#include <atomic>
#include <vector>
#include <memory>

#ifdef CHRONICLE_USE_SYSTEM_HYDRA

#include <hydra/activity.hpp>
#include <hydra/queue_batch.hpp>
#include <hydra/mpsc_queue.hpp>
#include <hydra/spsc_queue.hpp>

#else

#include "bundled/hydra/activity.hpp"
#include "bundled/hydra/queue_batch.hpp"
#include "bundled/hydra/mpsc_queue.hpp"
#include "bundled/hydra/spsc_queue.hpp"

#endif // CHRONICLE_USE_SYSTEM_HYDRA


#ifdef CHRONICLE_USE_SYSTEM_UFORMAT

#include <uformat/texter.hpp>

#else

#include "bundled/uformat/texter.hpp"

#endif // CHRONICLE_USE_SYSTEM_UFORMAT


#include "traits.hpp"
#include "severity.hpp"
#include "message.hpp"
#include "sink.hpp"


namespace chronicle {


template<typename Tr>
struct basic_data_log {

  using data_type = typename Tr::data_type;
  using format_type = typename Tr::format_type;
  using queue_type = typename Tr::queue_type;
  using message_type = message<data_type>;
  using activity_type = hydra::activity<message_type, queue_type>;
  using batch_type = typename activity_type::batch;
  using size_type = typename activity_type::size_type;
  using sinks_type = std::vector<std::unique_ptr<sink>>;


  static constexpr size_type default_queue_size = 8192;


  basic_data_log(size_type message_size) noexcept:
    message_size_{message_size}
  { }

  basic_data_log(basic_data_log const&) = delete;
  basic_data_log& operator = (basic_data_log const&) = delete;
  ~basic_data_log() { close(); }
  bool opened() const noexcept { return activity_.active(); }
  size_type blocks_count() const noexcept { return activity_.blocks_count(); }
  void severity(severity s) noexcept { severity_ = s; }
  enum severity severity() const noexcept { return severity_; }
  sinks_type const& sinks() const noexcept { return sinks_; }
  void prologue(std::string text) noexcept { prologue_ = std::move(text); }
  void epilogue(std::string text) noexcept { epilogue_ = std::move(text); }


  bool add_sink(std::unique_ptr<sink>&& sink_ptr) {
    if(!sink_ptr || !sink_ptr->ready())
      return false;
    sinks_.emplace_back(std::move(sink_ptr));
    return true;
  }


  template<typename Rep, typename Period>
  void flush_timeout(std::chrono::duration<Rep, Period> const& timeout) noexcept {
    flush_timeout_ = timeout;
  }


  std::chrono::system_clock::duration flush_timeout() const noexcept {
    return flush_timeout_;
  }
  
  
  bool open(std::unique_ptr<sink>&& sink_ptr, size_type queue_size = default_queue_size) {
    if(!add_sink(std::move(sink_ptr)))
      return false;
    return open(queue_size);
  }


  bool open(size_type queue_size = default_queue_size) {

    if(sinks_.empty())
      return false;
    for(auto const& each_sink: sinks_)
      if(!each_sink->ready())
        return false;

    if(!prologue_.empty())
      for(auto& each_sink: sinks_)
        each_sink->prologue(prologue_.data(), prologue_.size());

    activity_.reserve(queue_size);

    return activity_.run([this](auto& batch) {

      buffer_.clear();
      buffer_.reserve(message_size_ * batch.size());

      auto const now = std::chrono::system_clock::now();
      while(auto sequence = batch.try_fetch()) {
        message_type& message = batch[sequence];
        message.time = now;
        format_.print(message, buffer_);
        batch.fetched();        
      }

      bool have_to_flush;
      if(now - last_flush_time_ > flush_timeout_) {
        last_flush_time_ = now;
        have_to_flush = true;
      } else {
        have_to_flush = false;
      }

      for(auto& each_sink: sinks_) {
        each_sink->write(now, buffer_.data(), buffer_.size());
        if(have_to_flush)
          each_sink->flush();
      }
    });
  }


  void close() {
    if (!activity_.active())
      return;
    activity_.stop();
    if(!epilogue_.empty())
      for(auto& each_sink: sinks_)
        each_sink->epilogue(epilogue_.data(), epilogue_.size());
    for(auto& each_sink: sinks_)
      each_sink->close();
  }


  template<size_t N1, size_t N2> void failure(char const (&tag)[N1], char const (&text)[N2]) {
    print<severity::failure>(std::string_view{tag, N1 - 1}, std::string_view{text, N2 - 1});
  }


  template<size_t N1, size_t N2> void failure(char const (&tag)[N1], char const (&text)[N2], data_type const& data) {
    print<severity::failure>(std::string_view{tag, N1 - 1}, std::string_view{text, N2 - 1}, data);
  }


  template<size_t N1, size_t N2> void error(char const (&tag)[N1], char const (&text)[N2]) {
    print<severity::error>(std::string_view{tag, N1 - 1}, std::string_view{text, N2 - 1});
  }


  template<size_t N1, size_t N2> void error(char const (&tag)[N1], char const (&text)[N2], data_type const& data) {
    print<severity::error>(std::string_view{tag, N1 - 1}, std::string_view{text, N2 - 1}, data);
  }


  template<typename R, size_t N1, size_t N2> R error_with(R&& r, char const (&tag)[N1], char const (&text)[N2]) {
    print<severity::error>(std::string_view{tag, N1 - 1}, std::string_view{text, N2 - 1});
    return std::forward<R>(r);
  }


  template<typename R, size_t N1, size_t N2> R error_with(R&& r, char const (&tag)[N1], char const (&text)[N2], data_type const& data) {
    print<severity::error>(std::string_view{tag, N1 - 1}, std::string_view{text, N2 - 1}, data);
    return std::forward<R>(r);
  }



  template<size_t N1, size_t N2> void warning(char const (&tag)[N1], char const (&text)[N2]) {
    print<severity::warning>(std::string_view{tag, N1 - 1}, std::string_view{text, N2 - 1});
  }


  template<size_t N1, size_t N2> void warning(char const (&tag)[N1], char const (&text)[N2], data_type const& data) {
    print<severity::warning>(std::string_view{tag, N1 - 1}, std::string_view{text, N2 - 1}, data);
  }


  template<size_t N1, size_t N2> void info(char const (&tag)[N1], char const (&text)[N2]) {
    print<severity::info>(std::string_view{tag, N1 - 1}, std::string_view{text, N2 - 1});
  }


  template<size_t N1, size_t N2> void info(char const (&tag)[N1], char const (&text)[N2], data_type const& data) {
    print<severity::info>(std::string_view{tag, N1 - 1}, std::string_view{text, N2 - 1}, data);
  }


  template<size_t N1, size_t N2> void extra(char const (&tag)[N1], char const (&text)[N2]) {
    print<severity::extra>(std::string_view{tag, N1 - 1}, std::string_view{text, N2 - 1});
  }


  template<size_t N1, size_t N2> void extra(char const (&tag)[N1], char const (&text)[N2], data_type const& data) {
    print<severity::extra>(std::string_view{tag, N1 - 1}, std::string_view{text, N2 - 1}, data);
  }


  template<size_t N1, size_t N2> void trace(char const (&tag)[N1], char const (&text)[N2]) {
    print<severity::trace>(std::string_view{tag, N1 - 1}, std::string_view{text, N2 - 1});
  }


  template<size_t N1, size_t N2> void trace(char const (&tag)[N1], char const (&text)[N2], data_type const& data) {
    print<severity::trace>(std::string_view{tag, N1 - 1}, std::string_view{text, N2 - 1}, data);
  }


#ifdef NDEBUG


  template<size_t N1, size_t N2> void debug(char const (&)[N1], char const (&)[N2]) {
  }


  template<size_t N1, size_t N2> void debug(char const (&)[N1], char const (&)[N2], data_type const&) {
  }


#else


  template<size_t N1, size_t N2> void debug(char const (&tag)[N1], char const (&text)[N2]) {
    print<severity::debug>(std::string_view{tag, N1 - 1}, std::string_view{text, N2 - 1});
  }


  template<size_t N1, size_t N2> void debug(char const (&tag)[N1], char const (&text)[N2], data_type const& data) {
    print<severity::debug>(std::string_view{tag, N1 - 1}, std::string_view{text, N2 - 1}, data);
  }


#endif


protected:

  template<chronicle::severity S> void print(std::string_view const& tag,
                                  std::string_view const& text) {
    if(severity_ < S)
      return;
    message_type* m = claim<S>(tag, text);
    if(!m)
      return;
    publish(*m);
  }



  template<chronicle::severity S> void print(std::string_view const& tag,
                                  std::string_view const& text,
                                  data_type const& data) {
    if(severity_ < S)
      return;
    message_type* m = claim<S>(tag, text);
    if(!m)
      return;
    m->data = data;
    m->has_data = true;
    publish(*m);
  }


  void publish(message_type const& m) {
    activity_.publish(m.sequence);
  }


  template<chronicle::severity S> message_type* claim(std::string_view const& source,
                                         std::string_view const& text) {
    auto const sequence = activity_.claim();
    if(!sequence)
      return nullptr;
    message_type& m = activity_[sequence];
    m.sequence = sequence;
    m.severity = S;
#if defined(_WIN32)
    m.thread_id = unsigned(GetCurrentThreadId());
#else
#error Unsupported system
#endif
    m.source = source;
    m.text = text;
    m.has_data = false;
    return &m;
  }


private:

  sinks_type sinks_;
  enum severity severity_{chronicle::severity::info};
  activity_type activity_;
  size_type message_size_;
  uformat::dynamic_texter buffer_;
  std::chrono::system_clock::duration flush_timeout_;
  std::chrono::system_clock::time_point last_flush_time_;
  format_type format_;
  std::string prologue_{"\n LOG OPENED\n\n"};
  std::string epilogue_{"\n LOG CLOSED\n\n"};

}; // basic_data_log

template<typename D>
using unique_data_log_traits = traits_unique_default<D>;
template<typename D>
using shared_data_log_traits = traits_shared_default<D>;

template<typename D>
using unique_data_log = basic_data_log<unique_data_log_traits<D>>;
template<typename D>
using shared_data_log = basic_data_log<shared_data_log_traits<D>>;

} // chronicle
