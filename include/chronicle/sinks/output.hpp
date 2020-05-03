#pragma once


#ifdef _WIN32

#ifndef NOMINMAX
#define NOMINMAX
#endif

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN
#endif

#include <Windows.h>

#endif


namespace chronicle { namespace sinks {


#if defined(_WIN32)

  struct output {

    static output open() {
      return output(GetStdHandle(STD_OUTPUT_HANDLE));
    }

    output() noexcept = default;
    output(output const&) = delete;
    output& operator = (output const&) noexcept = delete;
    explicit operator bool () const noexcept { return handle_ != nullptr; }

    output(output&& other) noexcept: handle_{other.handle_} {
      other.handle_ = nullptr;
    }

    output& operator = (output&& other) noexcept {
      handle_ = other.handle_; other.handle_ = nullptr;
      return *this;
    }

    void write(char const* data, size_t size) {
      WriteFile(handle_, data, DWORD(size), nullptr, nullptr);
    }


    void close() {
      handle_ = nullptr;
    }

  private:

    void* handle_{nullptr};

    output(void* handle) noexcept: handle_{handle} { }

  }; // output


#else

#error Unsupported system

#endif


} } // sinks chronicle
