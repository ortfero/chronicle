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


#ifdef CHRONICLE_USE_SYSTEM_UFORMAT

#include <uformat/texter.hpp>

#else

#include "../bundled/uformat/texter.hpp"

#endif


#include "../message.hpp"


namespace chronicle::fields {
  
  
  class severity {
  public:
    
    template<class S, typename D>
    void print(message<D> const& m, uformat::texter<S>& texter) {
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
