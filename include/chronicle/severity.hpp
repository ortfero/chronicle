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


#include <string_view>
#include <string>


namespace chronicle {


  enum class severity {
    undefined, failure, error, warning, info, extra, trace, debug
  }; // severity
  
  
  inline bool filter_by_severity(severity of_logger, severity of_message) noexcept {
      if(of_message > of_logger)
          return true;
      return false;
  }


  inline severity parse_severity(std::string_view const& text) {
    if(text.empty())
      return severity::undefined;
    switch(text.size()) {
     case 4:  return (text == "info") ? severity::info : severity::undefined;
     case 5:  switch(text[0]) {
                case 'e':
                  if(text == "error") return severity::error;
                  if(text == "extra") return severity::extra;
                  return severity::undefined;
                case 't':
                  return (text == "trace") ? severity::trace : severity::undefined;
                case 'd':
                  return (text == "debug") ? severity::debug : severity::undefined;
                default:
                  return severity::undefined;
              }
     case 7:  switch(text[0]) {
                case 'f':
                  return (text == "failure") ? severity::failure : severity::undefined;
                case 'w':
                  return (text == "warning") ? severity::warning : severity::undefined;
                default:
                  return severity::undefined;
              }
     default: return severity::undefined;
    }
  }


  inline severity parse_severity(std::string const& text) {
    return parse_severity(std::string_view{text.data(), text.size()});
  }

} // chronicle
