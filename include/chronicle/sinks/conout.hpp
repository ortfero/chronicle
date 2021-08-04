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


#include <memory>


#if defined(_WIN32)

#if !defined(_X86_) && !defined(_AMD64_) && !defined(_ARM_) && !defined(_ARM64_)
#if defined(_M_IX86)
#define _X86_
#elif defined(_M_AMD64)
#define _AMD64_
#elif defined(_M_ARM)
#define _ARM_
#elif defined(_M_ARM64)
#define _ARM64_
#endif
#endif

#include <minwindef.h>
#include <ProcessEnv.h>
#include <fileapi.h>

#endif


#include "../sink.hpp"


namespace chronicle::sinks {


#if defined(_WIN32)

  class conout: public sink {
  public:

    static constexpr auto std_output_handle = DWORD(-11);

    static std::unique_ptr<sink> open() noexcept {
      return std::unique_ptr<sink>{new conout{GetStdHandle(std_output_handle)}};
    }

    conout() noexcept = default;
    conout(conout const&) = delete;
    conout& operator = (conout const&) noexcept = delete;    

    conout(conout&& other) noexcept: handle_{other.handle_} {
      other.handle_ = nullptr;
    }

    conout& operator = (conout&& other) noexcept {
      handle_ = other.handle_; other.handle_ = nullptr;
      return *this;
    }
    
    
    bool ready() const noexcept override {
      return handle_ != nullptr;
    }

    
    void write(time_point const&, char const* data, size_t size) noexcept override {
      WriteFile(handle_, data, DWORD(size), nullptr, nullptr);
    }
    
    
    void flush() noexcept override {
      FlushFileBuffers(handle_);
    }


    void close() noexcept override {
      handle_ = nullptr;
    }


    void prologue(const char*, size_t) noexcept override
    { }


    void epilogue(const char*, size_t) noexcept override
    { }


  private:

    void* handle_{nullptr};

    conout(void* handle) noexcept: handle_{handle} { }

  }; // conout


#else

#error Unsupported system

#endif


}// chronicle::sinks
