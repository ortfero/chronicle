// This file is part of chronicle library
// Copyright 2020-2022 Andrei Ilin <ortfero@gmail.com>
// SPDX-License-Identifier: MIT

#pragma once


#include <date/date.h>
#include <ufmt/text.hpp>

#include <chronicle/message.hpp>


namespace chronicle::fields {
  
  class utc_time_only_us {
  public:
  
    template<class S, typename D, class TimePoint>
    void print(message<D, TimePoint> const& m, ufmt::text<S>& text) {
      using namespace std::chrono;
      auto const dp = floor<date::days>(m.time);
      auto const tod = date::time_of_day{duration_cast<microseconds>(m.time - dp)};
                    
      if(tod.hours().count() < 10)
        text << '0';
      text << tod.hours().count();
      text << ':';

      if(tod.minutes().count() < 10)
        text << '0';
      text << tod.minutes().count();
      text << ':';

      if(tod.seconds().count() < 10)
        text << '0';
      text << tod.seconds().count();
      text << '.';

      auto const micros = tod.subseconds().count();
      text << ufmt::integer_fixed{micros, 6};
    }
    
  }; // utc_time_only_us
  
  
} // namespace chronicle::fields
