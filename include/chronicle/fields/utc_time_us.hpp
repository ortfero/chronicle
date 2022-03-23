// This file is part of chronicle library
// Copyright 2020-2022 Andrei Ilin <ortfero@gmail.com>
// SPDX-License-Identifier: MIT

#pragma once


#include <date/date.h>


#ifdef CHRONICLE_USE_SYSTEM_UFORMAT

#include <uformat/texter.hpp>

#else

#include "../bundled/uformat/texter.hpp"

#endif // CHRONICLE_USE_SYSTEM_UFORMAT


#include "../message.hpp"


namespace chronicle::fields {
  
    
  class utc_time_us {
  public:
  
    template<class S, typename D, class TimePoint>
    void print(message<D, TimePoint> const& m, uformat::texter<S>& texter) {
      using namespace std::chrono;
      auto const dp = floor<date::days>(m.time);
      date::year_month_day const ymd = dp;
      date::time_of_day<microseconds> const tod{duration_cast<microseconds>(m.time - dp)};
                    
      texter << int(ymd.year()) << '-';
      if(unsigned(ymd.month()) < 10)
        texter << '0';
      texter << unsigned(ymd.month());
      texter << '-';
      if(unsigned(ymd.day()) < 10)
        texter << '0';
      texter << unsigned(ymd.day());
      texter << ' ';

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

      auto const micros = tod.subseconds().count();
      texter.fixed(micros, 6);
    }
    
  }; // utc_time_us
  
  
} // chronicle::fields
