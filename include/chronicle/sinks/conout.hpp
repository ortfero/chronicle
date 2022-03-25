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

#elif defined(__linux__)

#include <sys/stat.h>
#include <fcntl.h>

#endif

#include <chronicle/sink.hpp>


namespace chronicle::sinks {


  class conout: public sink {

#if defined(_WIN32)
    using handle_type = void*;
    static constexpr auto invalid_handle = nullptr;
#elif defined(__linux__)
    using handle_type = int;
    static constexpr auto invalid_handle = -1;
#endif
    
    handle_type handle_{invalid_handle};

  public:

    static std::unique_ptr<sink> open() noexcept {
#if defined(_WIN32)
      return std::unique_ptr<sink>{new conout{GetStdHandle(DWORD(-11))}};
#elif defined(__linux__)
        return std::unique_ptr<sink>{new conout{1}};
#endif
    }

    conout() noexcept = default;
    conout(conout const&) = delete;
    conout& operator = (conout const&) noexcept = delete;    

    conout(conout&& other) noexcept: handle_{other.handle_} {
      other.handle_ = invalid_handle;
    }

    conout& operator = (conout&& other) noexcept {
      handle_ = other.handle_; other.handle_ = invalid_handle;
      return *this;
    }
    
    
    bool ready() const noexcept override {
      return handle_ != invalid_handle;
    }

    
    void write(time_point const&, char const* data, size_t size) noexcept override {
#if defined(_WIN32)
        WriteFile(handle_, data, DWORD(size), nullptr, nullptr);
#elif defined(__linux__)
        ::write(handle_, data, size);
#endif
    }
    
    
    void flush() noexcept override {
#if defined(_WIN32)
        FlushFileBuffers(handle_);
#elif defined(__linux__)
        ::fdatasync(handle_);
#endif
    }


    void close() noexcept override {
        handle_ = invalid_handle;
    }


    void prologue(const char*, size_t) noexcept override
    { }


    void epilogue(const char*, size_t) noexcept override
    { }


  private:

    explicit conout(handle_type handle) noexcept: handle_{handle} { }
  }; // conout


}// namespace chronicle::sinks
