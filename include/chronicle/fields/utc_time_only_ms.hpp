// This file is part of chronicle library
// Copyright 2020-2022 Andrei Ilin <ortfero@gmail.com>
// SPDX-License-Identifier: MIT

#pragma once


#include <chrono>


#include <date/date.h>


#ifdef CHRONICLE_USE_SYSTEM_UFORMAT

#include <uformat/texter.hpp>

#else

#include "../bundled/uformat/texter.hpp"

#endif // CHRONICLE_USE_SYSTEM_UFORMAT


#include "../message.hpp"


namespace chronicle::fields {


  class utc_time_only_ms {
  public:
  
    template<class S, typename D, class TimePoint>
    void print(message<D, TimePoint> const& m, uformat::texter<S>& texter) {
      using namespace std::chrono;
      auto const dp = floor<date::days>(m.time);
      date::time_of_day<milliseconds> const tod{duration_cast<milliseconds>(m.time - dp)};
                    
      if(tod.hours().count() < 10)
        texter << '0';
      texter << tod.hours().count();
      texter << ':';

      if(tod.minutes().count() < 10)
        texter << '0';
      texter << tod.minutes().count();
      texter << ':';

      if(tod.seconds().count() < 10)
        texter << '0';
      texter << tod.seconds().count();
      texter << '.';

      auto const millis = tod.subseconds().count();
      texter.fixed(millis, 3);
    }
    
  }; // utc_time_milliseconds
  
  
} // chronicle::fields
