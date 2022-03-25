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

#elif defined(__linux__)

#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

#endif

#include <date/date.h>
#include <ufmt/text.hpp>

#include <chronicle/sink.hpp>


namespace chronicle::sinks {


  class daily_rotated_file: public sink {
#if defined(_WIN32)
    using handle_type = void*;
    static constexpr auto invalid_handle = nullptr;
#elif defined(__linux__)
    using handle_type = int;
    static constexpr auto invalid_handle = -1;
#endif

    handle_type handle_{invalid_handle};
    std::filesystem::path directory_;
    std::filesystem::path base_name_;
    std::filesystem::path extension_;
    std::filesystem::path file_path_;
    uint64_t log_day_{0};
    unsigned part_{1};
    std::size_t limit_{0};
    std::size_t written_{0};
    
  public:

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
      other.handle_ = invalid_handle;
    }


    daily_rotated_file& operator = (daily_rotated_file&& other) noexcept {
      if(handle_ != invalid_handle)
#if defined(_WIN32)
        CloseHandle(handle_);
#elif defined(__linux__)
        ::close(handle_);
#endif
      handle_ = other.handle_; other.handle_ = invalid_handle;
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
      return handle_ != invalid_handle;
    }


    void write(time_point const& tp, char const* data, size_t size) noexcept override {
      using namespace std::chrono;
      std::error_code ec;
      auto const now_day = uint64_t(duration_cast<hours>(tp.time_since_epoch()).count() / 24);
      if(now_day != log_day_) {
        log_day_ = now_day;
        part_ = 1;
        rotate_file(tp, ec);
      } else if(limit_ != 0 && written_ + size > limit_) {
        ++part_;
        rotate_file(tp, ec);
      }
      
#if defined(_WIN32)
      WriteFile(handle_, data, DWORD(size), nullptr, nullptr);
#elif defined(__linux__)
      ::write(handle_, data, size);
#endif

      written_ += size;
    }
    
    
    void flush() noexcept override {
#if defined(_WIN32)
        FlushFileBuffers(handle_);
#elif defined(__linux__)
        ::fdatasync(handle_);
#endif
    }


    void close() noexcept override {
      if(handle_ == invalid_handle)
        return;
#if defined(_WIN32)
      CloseHandle(handle_);
#elif defined(__linux__)
      ::close(handle_);
#endif
      handle_ = invalid_handle;
    }


    void prologue(const char *data, size_t size) noexcept override {
#if defined(_WIN32)
      WriteFile(handle_, data, DWORD(size), nullptr, nullptr);
#elif defined(__linux__)
      ::write(handle_, data, size);
#endif
    }


    void epilogue(const char *data, size_t size) noexcept override {
#if defined(_WIN32)
      WriteFile(handle_, data, DWORD(size), nullptr, nullptr);
#elif defined(__linux__)
      ::write(handle_, data, size);
#endif
    }


  private:
    
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
      rotate_file(now, ec);
    }


    void rotate_file(std::chrono::system_clock::time_point tp, std::error_code& ec) {
      if(handle_ != invalid_handle) {
#if defined(_WIN32)
        CloseHandle(handle_);
#elif defined(__linux__)
        ::close(handle_);
#endif
      }
      auto const dp = std::chrono::floor<date::days>(tp);
      date::year_month_day const ymd = dp;
      ufmt::text suffix;
      suffix << '-' << int(ymd.year()) << '_';
      if(unsigned(ymd.month()) < 10)
        suffix << '0';
      suffix << unsigned(ymd.month()) << '_';
      if(unsigned(ymd.day()) < 10)
        suffix << '0';
      suffix << unsigned(ymd.day());
      if(part_ != 1) {
        suffix << '-';
        if(part_ < 10)
          suffix << '0';
        suffix << part_;
      }
      file_path_ = directory_;
      file_path_ /= base_name_;
      file_path_ += suffix.string();
      file_path_ += extension_;
#if defined(_WIN32)
    static constexpr auto file_append_data = DWORD(0x0004);
    static constexpr auto file_share_read = DWORD(0x00000001);
    static constexpr auto open_always = DWORD(4);
    static constexpr auto file_attribute_normal = DWORD(0x00000080);
    
      handle_ = CreateFileW(file_path_.native().data(), file_append_data, file_share_read, nullptr,
                                    open_always, file_attribute_normal, nullptr);
      if(handle_ == nullptr)
        ec = {int(GetLastError()), std::system_category()};
#elif defined(__linux__)
      handle_ = ::open(file_path_.native().data(), O_WRONLY | O_CREAT | O_APPEND | S_IWUSR | S_IROTH);
      if(handle_ == -1)
        ec = {errno, std::system_category()};
#endif
      written_ = 0;
    }


  }; // daily_rotated_file

} // namespace chronicle::sinks
