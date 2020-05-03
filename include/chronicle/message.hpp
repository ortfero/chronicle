#pragma once


#include <string_view>
#include <chrono>
#include "detail/hydra/sequence.hpp"
#include "detail/chineseroom/texter.hpp"
#include "severity.hpp"


namespace chronicle {


  template<typename D> struct message {

    hydra::sequence sequence;
    enum severity severity;
    std::chrono::system_clock::time_point time;
    unsigned thread_id;
    std::string_view tag;
    std::string_view text;
    bool has_data{false};
    D data;

    template<typename S> friend
    chineseroom::basic_texter<S>& operator << (chineseroom::basic_texter<S>& texter, message const& m) {
      switch(m.severity) {
        case severity::undefined:
          texter << "[undefined] "; break;
        case severity::fatal:
          texter << "[fatal] "; break;
        case severity::fail:
          texter << "[error] "; break;
        case severity::warning:
          texter << "[warning] "; break;
        case severity::notice:
          texter << "        "; break;
        case severity::debug:
          texter << "[debug] "; break;
        case severity::trace:
          texter << "[trace] "; break;
        default:
          texter << "[unknown] "; break;
      }

      texter << ' ' << '[' << m.thread_id << ']'
             << ' ' << '[' << m.tag << ']'
             << ' ' << m.text;

      if(m.has_data)
        texter << m.data;

      texter << '\n';

      return texter;
    }
  }; // message


} // chronicle
