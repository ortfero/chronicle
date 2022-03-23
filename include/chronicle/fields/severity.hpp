// This file is part of chronicle library
// Copyright 2020-2021 Andrei Ilin <ortfero@gmail.com>
// SPDX-License-Identifier: MIT

#pragma once


#include <ufmt/text.hpp>

#else

#include "../bundled/uformat/texter.hpp"

#endif


#include "../message.hpp"


namespace chronicle::fields {
  
  
  class severity {
  public:
    
    template<class S, typename D, class TimePoint>
    void print(message<D, TimePoint> const& m, uformat::texter<S>& texter) {
      switch(m.severity) {
        case chronicle::severity::undefined:
          texter << "-------"; break;
        case chronicle::severity::failure:
          texter << "failure"; break;
        case chronicle::severity::error:
          texter << "error  "; break;
        case chronicle::severity::warning:
          texter << "warning"; break;
        case chronicle::severity::info:
          texter << "       "; break;
        case chronicle::severity::extra:
          texter << "       "; break;
        case chronicle::severity::trace:
          texter << "trace  "; break;
        case chronicle::severity::debug:
          texter << "debug  "; break;
        default:
          texter << "unknown"; break;
      }
    }
    
  }; // severity
  
  
} // chronicle::fields
