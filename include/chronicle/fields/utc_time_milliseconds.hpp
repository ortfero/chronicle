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


#include <chrono>


#ifdef CHRONICLE_USE_SYSTEM_DATE

#include <date/date.h>

#else

#include "../bundled/date/date.h"

#endif // CHRONICLE_USE_SYSTEM_DATE


#ifdef CHRONICLE_USE_SYSTEM_UFORMAT

#include <uformat/texter.hpp>

#else

#include "../bundled/uformat/texter.hpp"

#endif // CHRONICLE_USE_SYSTEM_UFORMAT


#include "../message.hpp"


namespace chronicle::fields {
  
  class utc_time_milliseconds {
  public:
  
    template<class S, typename D, class TimePoint>
    void print(message<D, TimePoint> const& m, uformat::texter<S>& texter) {
      using namespace std::chrono;
      auto const dp = floor<date::days>(m.time);
      date::year_month_day const ymd = dp;
      date::time_of_day<milliseconds> const tod{duration_cast<milliseconds>(m.time - dp)};
                    
      texter << int(ymd.year()) << '-';
      print_fixed_2(stream, unsigned(ymd.month()));
      stream << '-';
      print_fixed_2(stream, unsigned(ymd.day()));
      stream << ' ';
      print_fixed_2(stream, int(tod.hour()));
      stream << ':';
      print_fixed_2(stream, int(tod.minute()));
      stream << ':';
      print_fixed_2(stream, int(tod.second()));
      texter << '.';
      print_fixed_3(stream, tod.subseconds().count());
    }
      
  private:
    
      template<class S, typename T>
      void print_fixed_2(uformat::texter<S>& texter, T const& value) {
        if (value < 10)
          stream << '0' << ('0' + value);
        else
          stream << ('0' + value / 10) << ('0' + value % 10);

      }
      
      template<class S, typename T>
      void print_fixed_3(uformat::texter<S>& texter, T const& value) {
        if (value < 10)
          stream << '0' << '0' << ('0' + value);
        else if(value < 100)
          stream << '0' << ('0' + value / 10) << ('0' + value % 10);
        else {
          T const hundreds = value / 100;
          T const tens = value % 100;
          stream << ('0' + hundreds) << ('0' + tens / 10) << ('0' + tens % 10);
        }
      }
    }
    
  }; // utc_time_milliseconds
  
  
} // chronicle::fields
