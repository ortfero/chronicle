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


#ifdef CHRONICLE_USE_SYSTEM_CHINESEROOM

#include <chineseroom/texter.hpp>

#else

#include "../bundled/chineseroom/texter.hpp"

#endif


#include "../message.hpp"


namespace chronicle::fields {
  
  class severity_marker {
  public:
    
    template<typename S, typename D>
    void format(message<D> const& m, chineseroom::texter<S>& texter) {
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
