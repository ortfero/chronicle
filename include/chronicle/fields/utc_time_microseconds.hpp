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


#include <date/date.h>


#ifdef CHRONICLE_USE_SYSTEM_UFORMAT

#include <uformat/texter.hpp>

#else

#include "../bundled/uformat/texter.hpp"

#endif // CHRONICLE_USE_SYSTEM_UFORMAT


#include "../message.hpp"


namespace chronicle::fields {
  
  class utc_time_microseconds {
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
    
  }; // utc_time_microseconds
  
  
} // chronicle::fields
