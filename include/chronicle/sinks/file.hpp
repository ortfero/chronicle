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

#endif


#include "../sink.hpp"


namespace chronicle { namespace sinks {


#if defined(_WIN32)

  class file: public sink {
  public:

    static constexpr auto file_append_data = DWORD(0x0004);
    static constexpr auto file_share_read = DWORD(0x00000001);
    static constexpr auto open_always = DWORD(4);
    static constexpr auto file_attribute_normal = DWORD(0x00000080);


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
    
    
    file(std::filesystem::path const& path, std::error_code& error) noexcept {
      auto const directory = path.parent_path();
      namespace fs = std::filesystem;
      if(!fs::exists(directory))
        fs::create_directories(directory, error);
      if(!!error)
        return;
      handle_ = CreateFileW(path.native().data(), file_append_data, file_share_read,
                            nullptr, open_always, file_attribute_normal, nullptr);
      if(handle_ == nullptr)
        error = {int(GetLastError()), std::system_category()};
    }


    file(file&& other) noexcept: handle_{other.handle_} {
      other.handle_ = nullptr;
    }


    file& operator = (file&& other) noexcept {
      if(handle_ != nullptr)
        CloseHandle(handle_);
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

  }; // file

#else

#error Unsupported system

#endif


} }
