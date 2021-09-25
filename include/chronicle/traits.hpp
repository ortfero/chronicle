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


#ifdef CHRONICLE_USE_SYSTEM_HYDRA

#include <hydra/mpsc_queue.hpp>
#include <hydra/spsc_queue.hpp>

#else

#include "bundled/hydra/mpsc_queue.hpp"
#include "bundled/hydra/spsc_queue.hpp"

#endif // CHRONICLE_USE_SYSTEM_HYDRA


#ifdef CHRONICLE_USE_SYSTEM_UFORMAT

#include <uformat/texter.hpp>

#else

#include "bundled/uformat/texter.hpp"

#endif // CHRONICLE_USE_SYSTEM_UFORMAT


#include "fields/default_format.hpp"
#include "message.hpp"


namespace chronicle {
  
  
  template<typename D, class Q, class F, class C>
  struct basic_traits {
    using data_type = D;
    using queue_type = Q;
    using format_type = F;
    using clock_type = C;
  }; // basic_traits
  

  template<typename D, class F, class C>
  using traits_unique = basic_traits<D, hydra::spsc_queue<message<D, typename C::time_point>>, F, C>;

  template<typename D, class F, class C>
  using traits_shared = basic_traits<D, hydra::mpsc_queue<message<D, typename C::time_point>>, F, C>;
  
  template<typename D, class C = std::chrono::system_clock>
  using traits_unique_default = traits_unique<D, fields::format_singlethreaded_default, C>;
  
  template<typename D, class C = std::chrono::system_clock>
  using traits_shared_default = traits_shared<D, fields::format_multithreaded_default, C>;
  
  template<typename D, class C = std::chrono::system_clock>
  using traits_unique_ms = traits_unique<D, fields::format_singlethreaded_ms, C>;
  
  template<typename D, class C = std::chrono::system_clock>
  using traits_shared_ms = traits_shared<D, fields::format_multithreaded_ms, C>;

  template<typename D, class C = std::chrono::system_clock>
  using traits_unique_us = traits_unique<D, fields::format_singlethreaded_us, C>;
  
  template<typename D, class C = std::chrono::system_clock>
  using traits_shared_us = traits_shared<D, fields::format_multithreaded_us, C>;
  
  template<typename D, class C = std::chrono::system_clock>
  using traits_unique_time_only_ms = traits_unique<D, fields::format_singlethreaded_time_only_ms, C>;

  template<typename D, class C = std::chrono::system_clock>
  using traits_shared_time_only_ms = traits_unique<D, fields::format_multithreaded_time_only_ms, C>;
 
  template<typename D, class C = std::chrono::system_clock>
  using traits_unique_time_only_us = traits_unique<D, fields::format_singlethreaded_time_only_us, C>;

  template<typename D, class C = std::chrono::system_clock>
  using traits_shared_time_only_us = traits_shared<D, fields::format_multithreaded_time_only_us, C>;
  

}
