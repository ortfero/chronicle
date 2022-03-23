// This file is part of chronicle library
// Copyright 2020-2022 Andrei Ilin <ortfero@gmail.com>
// SPDX-License-Identifier: MIT

#pragma once


#include <memory>
#include <filesystem>
#include <system_error>


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
#include <errhandlingapi.h>

#elif defined(__linux__)

#include <unistd.h>
#include <errno.h>

#else

#error Unsupported system

#endif


#include <chronicle/sink.hpp>


namespace chronicle::sinks {


  class file: public sink {
#if defined(_WIN32)
    using handle_type = void*;
    static constexpr auto invalid_handle = nullptr;
#elif defined(__linux__)
    using handle_type = int;
    static constexpr auto invalid_handle = -1;
#endif

    handle_type handle_{invalid_handle};
     
  public:

    static std::unique_ptr<sink> open(std::filesystem::path const& path,
                                      std::error_code& error) noexcept {
      file f{path, error};
      if(!f.ready())
        return nullptr;
      return std::unique_ptr<sink>{new file{std::move(f)}};
    }


    file() noexcept = default;
    ~file() override { close(); }

    file(file const&) noexcept = delete;
    file& operator = (file const&) noexcept = delete;

    file(file&& other) noexcept: handle_{other.handle_} {
      other.handle_ = invalid_handle;
    }


    file& operator = (file&& other) noexcept {
      if(handle_ != invalid_handle)
#if defined(_WIN32)
        CloseHandle(handle_);
#elif defined(__linux__)
        close(handle_);
#endif
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
#elif
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
    
    file(std::filesystem::path const& path, std::error_code& error) noexcept {
      auto const directory = path.parent_path();
      namespace fs = std::filesystem;
      if(!directory.empty() && !fs::exists(directory))
        fs::create_directories(directory, error);
      if(!!error)
        return;
#if defined(_WIN32)
    static constexpr auto file_append_data = DWORD(0x0004);
    static constexpr auto file_share_read = DWORD(0x00000001);
    static constexpr auto open_always = DWORD(4);
    static constexpr auto file_attribute_normal = DWORD(0x00000080);
    
      handle_ = CreateFileW(path.native().data(), file_append_data, file_share_read,
                            nullptr, open_always, file_attribute_normal, nullptr);
      if(handle_ == nullptr)
        error = {int(GetLastError()), std::system_category()};
#elif defined(__linux__)
      handle_ = ::open(path.native().data(), O_WRONLY | O_CREAT | O_APPEND | S_IWUSR | S_IROTH);
      if(handle_ == -1)
        error = {errno, std::system_category()};
#endif
    }

  }; // file

#else

#error Unsupported system

#endif


} // chronicle::sinks
