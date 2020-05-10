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


#include "format.hpp"
#include "severity.hpp"
#include "utc_time_microseconds.hpp"
#include "utc_time_milliseconds.hpp"
#include "thread_id.hpp"
#include "source.hpp"


namespace chronicle { namespace fields {
  
  
  using format_microseconds = format<severity, utc_time_microseconds, thread_id, source>;
  using format_milliseconds = format<severity, utc_time_milliseconds, thread_id, source>;
  using format_microseconds_single = format<severity, utc_time_microseconds, source>;
  using format_milliseconds_single = format<severity, utc_time_milliseconds, source>;
  
  using format_default = format_microseconds;
  using format_default_single = format_microseconds_single;
  
  
} }
