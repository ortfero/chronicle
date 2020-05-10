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


#include <tuple>


#ifdef CHRONICLE_USE_SYSTEM_UFORMAT

#include <uformat/texter.hpp>

#else

#include "../bundled/uformat/texter.hpp"

#endif


#include "../message.hpp"


namespace chronicle { namespace fields {
  
  
  template<class... Fields>
  class format {
  public:

    using fields_type = std::tuple<Fields...>;

    
    template<class S, typename D>
    void print(message<D> const& m, uformat::texter<S>& texter) {

      print_fields<S, D, 0>(m, texter);
      texter << ' ';
      texter << m.text;
      
      if(m.has_data)
        texter << m.data;
      
      texter << '\n';
    }
    
    
  private:
  
    fields_type fields_;

    template<class S, typename D, size_t I>
    void print_fields(message<D> const& message, uformat::texter<S>& texter) {

      texter << ' ';
      std::get<I>(fields_).print(message, texter);

      if constexpr (I + 1 != std::tuple_size_v<fields_type>)
        print_fields<S, D, I + 1>(message, texter);
    }
    
  }; // format



} }
