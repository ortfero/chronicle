#pragma once


#include <thread>
#include <atomic>
#include "detail/hydra/activity.hpp"
#include "detail/hydra/queue_batch.hpp"
#include "detail/hydra/mpsc_queue.hpp"
#include "detail/hydra/spsc_queue.hpp"
#include "detail/chineseroom/texter.hpp"
#include "sinks/file.hpp"
#include "severity.hpp"
#include "message.hpp"


#include <functional>


namespace chronicle {


template<typename S, typename D, typename Q = hydra::mpsc_queue<message<D>>>
struct basic_data_log {

  using message_type = message<D>;
  using sink_type = S;
  using activity = hydra::activity<message_type, Q>;
  using batch = typename activity::batch;
  using size_type = typename activity::size_type;


  static constexpr size_type default_buffer_size = 8192;


  basic_data_log(size_type buffer_for_message) noexcept:
    buffer_for_message_{buffer_for_message}
  { }

  basic_data_log(basic_data_log const&) = delete;
  basic_data_log& operator = (basic_data_log const&) = delete;
  ~basic_data_log() { activity_.stop(); }
  bool opened() const noexcept { return activity_.active(); }
  size_type blocks_count() const noexcept { return activity_.blocks_count(); }


  bool open(S&& sink, size_type buffer_size = default_buffer_size) {
    if(!sink)
      return false;
    sink_ = std::move(sink);
    activity_.reserve(buffer_size);

    return activity_.run([this](auto& batch) {
      buffer_.clear();
      buffer_.reserve(buffer_for_message_ * batch.size());
      auto const now = std::chrono::system_clock::now();
      while(auto sequence = batch.try_fetch()) {
        message_type& message = batch[sequence];
        message.time = now;
        buffer_ << message;
        buffer_ << '\n';
        batch.fetched();        
      }
      sink_.write(now, buffer_.data(), buffer_.size());
    });
  }


  void close() {
    activity_.stop();
    sink_.close();
  }


  template<size_t N1, size_t N2> void notice(char const (&tag)[N1], char const (&text)[N2]) {
    print<severity::notice>(std::string_view{tag, N1 - 1}, std::string_view{text, N2 - 1});
  }


  template<size_t N1, size_t N2> void notice(char const (&tag)[N1], char const (&text)[N2], D const& data) {
    print<severity::notice>(std::string_view{tag, N1 - 1}, std::string_view{text, N2 - 1}, data);
  }


protected:

  template<severity S> void print(std::string_view const& tag,
                                  std::string_view const& text) {
    message<D>* m = claim<S>(tag, text);
    if(!m)
      return;
    publish(*m);
  }



  template<severity S> void print(std::string_view const& tag,
                                  std::string_view const& text,
                                  D const& data) {
    message<D>* m = claim<S>(tag, text);
    if(!m)
      return;
    m->data = data;
    m->has_data = true;
    publish(*m);
  }


  void publish(message<D> const& m) {
    activity_.publish(m.sequence);
  }


  template<severity S> message<D>* claim(std::string_view const& tag,
                                         std::string_view const& text) {
    auto const sequence = activity_.claim();
    if(!sequence) {
      //fputs("claim lock", stderr);
      /*printf("claim failed: producer = %lld, consumer = %lld\n",
             activity_.messages_.producer_.load(),
             activity_.messages_.consumer_);*/
      return nullptr;
    }
    message<D>& m = activity_[sequence];
    m.sequence = sequence;
    m.severity = S;
#if defined(_WIN32)
    m.thread_id = unsigned(GetCurrentThreadId());
#else
#error Unsupported system
#endif
    m.tag = tag;
    m.text = text;
    m.has_data = false;
    return &m;
  }


private:

  S sink_;
  activity activity_;
  size_type buffer_for_message_;
  chineseroom::large_texter buffer_;
  int value_;

}; // log


template<typename S, typename D> using
  data_log_mt = basic_data_log<S, D>;

template<typename S, typename D> using
  data_log = basic_data_log<S, D, hydra::spsc_queue<message<D>>>;

template<typename D> using
  file_data_log_mt = data_log_mt<sinks::file, D>;

template<typename D> using
  file_data_log = data_log<sinks::file, D>;

} // chronicle
