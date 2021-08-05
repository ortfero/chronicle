/* This file is part of chronicle library
 * Copyright (c) 2020-2021 Andrei Ilin <ortfero@gmail.com>
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


#include <string_view>
#include <chrono>


#ifdef CHRONICLE_USE_SYSTEM_HYDRA

#include <hydra/sequence.hpp>

#else

#include "bundled/hydra/sequence.hpp"

#endif // CHRONICLE_USE_SYSTEM_HYDRA


#ifdef CHRONICLE_USE_SYSTEM_DATE

#include <date/date.h>

#else

#include "bundled/date/date.h"

#endif


#include "severity.hpp"


namespace chronicle {


  template<typename D> struct message {

    hydra::sequence sequence;
    enum severity severity;
    std::chrono::system_clock::time_point time;
    unsigned thread_id;
    std::string_view source;
    std::string_view text;
    bool has_data{false};
    D data;

  }; // message


} // chronicle
