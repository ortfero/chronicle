// This file is part of chronicle library
// Copyright 2020-2022 Andrei Ilin <ortfero@gmail.com>
// SPDX-License-Identifier: MIT

#pragma once


#ifdef CHRONICLE_USE_SYSTEM_CHINESEROOM

#include <chineseroom/texter.hpp>

#else

#include "../bundled/chineseroom/texter.hpp"

#endif


#include "../message.hpp"


namespace chronicle::fields {
  
  class severity_marker {
  public:
    
    template<typename S, typename D, class TimePoint>
    void format(message<D, TimePoint> const& m, chineseroom::texter<S>& texter) {
      switch(m.severity) {
        case severity::undefined:
          texter << '[' << '?' << ']'; return;
        case severity::failure:
          texter << '[' << 'F' << ']'; return;
        case severity::error:
          texter << '[' << 'E' << ']'; return;
        case severity::warning:
          texter << '[' << 'W' << ']'; return;
        case severity::info:
          texter << ' ' << ' ' << ' ' ; return;
        case severity::extra:
          texter << ' ' << ' ' << ' '; return;
        case severity::trace:
          texter << '[' << 'T' << ']'; return;
        case severity::debug:
          texter << '[' << 'D' << ']'; return;
        default:
          texter << '[' << '?' << ']'; return;
      }
    }
    
  }; // severity_marker

} // chronicle::fields
