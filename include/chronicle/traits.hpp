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


#ifdef CHRONICLE_USE_SYSTEM_THEATER

#include <theater/mpsc_queue.hpp>
#include <theater/spsc_queue.hpp>

#else

#include "bundled/theater/mpsc_queue.hpp"
#include "bundled/theater/spsc_queue.hpp"

#endif // CHRONICLE_USE_SYSTEM_THEATER


#ifdef CHRONICLE_USE_SYSTEM_UFORMAT

#include <uformat/texter.hpp>

#else

#include "bundled/uformat/texter.hpp"

#endif // CHRONICLE_USE_SYSTEM_UFORMAT


#include "fields/default_format.hpp"
#include "message.hpp"


namespace chronicle {
  
  
  template<typename D, class Q, class F>
  struct basic_traits {
    using data_type = D;
    using queue_type = Q;
    using format_type = F;
  }; // basic_traits
  

  template<typename D, class F>
  using traits_single = basic_traits<D, theater::spsc_queue<message<D>>, F>;

  template<typename D, class F>
  using traits = basic_traits<D, theater::mpsc_queue<message<D>>, F>;
  
  template<typename D>
  using traits_default_single = traits_single<D, fields::format_default_single>;
  
  template<typename D>
  using traits_default = traits<D, fields::format_default>;
  
  template<typename D>
  using traits_milliseconds_single = traits_single<D, fields::format_milliseconds_single>;
  
  template<typename D>
  using traits_milliseconds = traits<D, fields::format_milliseconds>;

  template<typename D>
  using traits_microseconds_single = traits_single<D, fields::format_microseconds_single>;
  
  template<typename D>
  using traits_microseconds = traits<D, fields::format_microseconds>;


}
