// This file is part of chronicle library
// Copyright 2020-2022 Andrei Ilin <ortfero@gmail.com>
// SPDX-License-Identifier: MIT

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

  class conerr: public sink {
  public:

    static constexpr auto std_error_handle = DWORD(-12);

    static std::unique_ptr<sink> open() noexcept {
      return std::unique_ptr<sink>{new conerr{GetStdHandle(std_error_handle)}};
    }

    conerr() noexcept = default;
    conerr(conerr const&) = delete;
    conerr& operator = (conerr const&) noexcept = delete;

    conerr(conerr&& other) noexcept: handle_{other.handle_} {
      other.handle_ = nullptr;
    }

    conerr& operator = (conerr&& other) noexcept {
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

    conerr(void* handle) noexcept: handle_{handle} { }

  }; // conerr


#else

#error Unsupported system

#endif


} // chronicle::sinks
