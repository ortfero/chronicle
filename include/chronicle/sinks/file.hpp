#pragma once


#if defined(_WIN32)

#ifndef NOMINMAX
#define NOMINMAX
#endif

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN
#endif

#include <Windows.h>

#endif


#include <chrono>


namespace chronicle { namespace sinks {


#if defined(_WIN32)

  struct file {

    static file open(char const* path) {
      void* const handle = CreateFileA(path, FILE_APPEND_DATA, FILE_SHARE_READ, nullptr,
                                    OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
      return file{handle};
    }


    file() noexcept = default;
    file(file const&) noexcept = delete;
    file& operator = (file const&) noexcept = delete;
    explicit operator bool () const noexcept { return handle_ != nullptr; }

    ~file() {
      if(handle_ != nullptr)
        CloseHandle(handle_);
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

    void write(std::chrono::system_clock::time_point, char const* data, size_t size) {
      WriteFile(handle_, data, DWORD(size), nullptr, nullptr);
    }

    void close() {
      if(handle_ == nullptr)
        return;
      CloseHandle(handle_);
      handle_ = nullptr;
    }

  private:

    void* handle_{nullptr};

    file(void* handle) noexcept: handle_{handle} { }


  }; // file

#else

#error Unsupported system

#endif


} }
