// This file is part of chronicle library
// Copyright 2020-2022 Andrei Ilin <ortfero@gmail.com>
// SPDX-License-Identifier: MIT

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


#include <date/date.h>


#ifdef CHRONICLE_USE_SYSTEM_UFORMAT

#include <uformat/texter.hpp>

#else

#include "../bundled/uformat/texter.hpp"

#endif


#include "../sink.hpp"


namespace chronicle::sinks {


#if defined(_WIN32)

  class daily_rotated_file: public sink {
  public:

    static constexpr auto file_append_data = DWORD(0x0004);
    static constexpr auto file_share_read = DWORD(0x00000001);
    static constexpr auto open_always = DWORD(4);
    static constexpr auto file_attribute_normal = DWORD(0x00000080);


    static std::unique_ptr<daily_rotated_file> open(std::filesystem::path const& path,
                                                    std::error_code& ec,
                                                    std::size_t limit = 0) {
      std::unique_ptr<daily_rotated_file> p{new daily_rotated_file{path, limit, ec}};
      if(!p->ready())
        return nullptr;
      return p;
    }


    daily_rotated_file() noexcept = default;
    ~daily_rotated_file() override { close(); }
    daily_rotated_file(daily_rotated_file const&) noexcept = delete;
    daily_rotated_file& operator = (daily_rotated_file const&) noexcept = delete;
    std::filesystem::path const& file_path() const noexcept { return file_path_; }


    daily_rotated_file(std::filesystem::path const& path,
                       std::size_t limit,
                       std::error_code& ec) noexcept {
      namespace fs = std::filesystem;
      directory_ = path.parent_path();
      if(!directory_.empty() && !fs::exists(directory_))
        fs::create_directories(directory_, ec);
      if(!!ec)
        return;
      base_name_ = path.stem();
      extension_ = path.extension();
      using namespace std::chrono;
      auto const now = system_clock::now();
      log_day_ = duration_cast<hours>(now.time_since_epoch()).count() / 24;
      limit_ = limit;

      rotate_file(now);
    }


    daily_rotated_file(daily_rotated_file&& other) noexcept:
      handle_{ other.handle_ },
      directory_{ std::move(other.directory_) },
      base_name_{ std::move(other.base_name_) },
      extension_{ std::move(other.extension_) },
      file_path_{ std::move(other.file_path_) },
      log_day_{ other.log_day_ },
      part_{ other.part_ },
      limit_{ other.limit_ },
      written_{ other.written_ } {
      other.handle_ = nullptr;
    }


    daily_rotated_file& operator = (daily_rotated_file&& other) noexcept {
      if(handle_ != nullptr)
        CloseHandle(handle_);
      handle_ = other.handle_; other.handle_ = nullptr;
      directory_ = std::move(other.directory_);
      base_name_ = std::move(other.base_name_);
      extension_ = std::move(other.extension_);
      file_path_ = std::move(other.file_path_);
      log_day_ = other.log_day_;
      part_ = other.part_;
      limit_ = other.limit_;
      written_ = other.written_;
      return *this;
    }
    
    
    bool ready() const noexcept override {
      return handle_ != nullptr;
    }


    void write(time_point const& tp, char const* data, size_t size) noexcept override {
      using namespace std::chrono;
      auto const now_day = uint64_t(duration_cast<hours>(tp.time_since_epoch()).count() / 24);
      if(now_day != log_day_) {
        log_day_ = now_day;
        part_ = 1;
        rotate_file(tp);
      } else if(limit_ != 0 && written_ + size > limit_) {
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


}
