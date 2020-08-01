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


#include <chrono>
#include <filesystem>
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
#include <fileapi.h>
#include <handleapi.h>
#include <sysinfoapi.h>

#endif


#ifdef CHRONICLE_USE_SYSTEM_DATE

#include <date/date.h>

#else

#include "../bundled/date/date.h"

#endif // CHRONICLE_USE_SYSTEM_DATE


#ifdef CHRONICLE_USE_SYSTEM_UFORMAT

#include <uformat/texter.hpp>

#else

#include "../bundled/uformat/texter.hpp"

#endif


#include "../sink.hpp"


namespace chronicle { namespace sinks {


#if defined(_WIN32)

  class daily_rotated_file: public sink {
  public:

    static constexpr auto file_append_data = DWORD(0x0004);
    static constexpr auto file_share_read = DWORD(0x00000001);
    static constexpr auto open_always = DWORD(4);
    static constexpr auto file_attribute_normal = DWORD(0x00000080);


    static std::unique_ptr<sink> open(std::filesystem::path const& path, std::size_t limit = 0) {
      daily_rotated_file f{path, limit};
      if(!f.ready())
        return nullptr;
      return std::unique_ptr<sink>{new daily_rotated_file{std::move(f)}};
    }


    daily_rotated_file() noexcept = default;
    ~daily_rotated_file() override { close(); }
    daily_rotated_file(daily_rotated_file const&) noexcept = delete;
    daily_rotated_file& operator = (daily_rotated_file const&) noexcept = delete;
    std::filesystem::path const& file_path() const noexcept { return file_path_; }


    daily_rotated_file(std::filesystem::path const& path, std::size_t limit) noexcept {
      namespace fs = std::filesystem;
      directory_ = path.parent_path();
      std::error_code failed;
      if(!fs::exists(directory_))
        fs::create_directories(directory_, failed);
      if(!!failed)
        return;
      base_name_ = path.stem();
      extension_ = path.extension();
      using namespace std::chrono;
      auto const now = system_clock::now();
      log_day_ = duration_cast<hours>(now.time_since_epoch()).count() / 24;
      limit_ = limit;
      rotate_file(now);
    }


    daily_rotated_file(daily_rotated_file&& other) noexcept: handle_{other.handle_} {
      other.handle_ = nullptr;
    }


    daily_rotated_file& operator = (daily_rotated_file&& other) noexcept {
      if(handle_ != nullptr)
        CloseHandle(handle_);
      handle_ = other.handle_; other.handle_ = nullptr;
      return *this;
    }
    
    
    bool ready() const noexcept override {
      return handle_ != nullptr;
    }


    void write(time_point const& tp, char const* data, size_t size) noexcept override {
      using namespace std::chrono;
      auto const now_day = duration_cast<hours>(tp.time_since_epoch()).count() / 24;
      if(now_day != log_day_) {
        log_day_ = now_day;
        part_ = 1;
        rotate_file(tp);
      }
      if(limit_ != 0 && written_ + size > limit_) {
        ++part_;
        rotate_file(tp);
      }
      
      WriteFile(handle_, data, DWORD(size), nullptr, nullptr);
      written_ += size;
    }
    
    
    void flush() noexcept override {
      FlushFileBuffers(handle_);
    }


    void close() noexcept override {
      if(handle_ == nullptr)
        return;
      CloseHandle(handle_);
      handle_ = nullptr;
    }


    void prologue(const char *data, size_t size) noexcept override {
      WriteFile(handle_, data, DWORD(size), nullptr, nullptr);
    }


    void epilogue(const char *data, size_t size) noexcept override {
      WriteFile(handle_, data, DWORD(size), nullptr, nullptr);
    }


  private:

    void* handle_{nullptr};
    std::filesystem::path directory_;
    std::filesystem::path base_name_;
    std::filesystem::path extension_;
    std::filesystem::path file_path_;
    uint64_t log_day_{0};
    unsigned part_{1};
    std::size_t limit_{0};
    std::size_t written_{0};


    void rotate_file(std::chrono::system_clock::time_point tp) {
      if(handle_ != nullptr) {
        CloseHandle(handle_);
        handle_ = nullptr;
      }
      auto const dp = std::chrono::floor<date::days>(tp);
      date::year_month_day const ymd = dp;
      uformat::short_texter texter;
      texter.print('-', int(ymd.year()), '_')
          .fixed(unsigned(ymd.month()), 2).print('_')
          .fixed(unsigned(ymd.day()), 2);
      if(part_ != 1)
        texter.print('-').fixed(part_, 2);
      file_path_ = directory_;
      file_path_ /= base_name_;
      file_path_ += texter.string().data();
      file_path_ += extension_;
      handle_ = CreateFileW(file_path_.native().data(), file_append_data, file_share_read, nullptr,
                                    open_always, file_attribute_normal, nullptr);
      written_ = 0;
    }


  }; // file

#else

#error Unsupported system

#endif


} }
