// This file is part of chronicle library
// Copyright 2020-2022 Andrei Ilin <ortfero@gmail.com>
// SPDX-License-Identifier: MIT

#pragma once


#include <cstdio>
#include <memory>

#include <chronicle/sink.hpp>


namespace chronicle::sinks {



    class conerr: public sink {
    public:
        static expected_sink_ptr open() noexcept {
            return {sink_ptr{new conerr{}}};
        }

        bool ready() const noexcept override {
            return true;
        }


        void write(time_point const&,
                   char const* data,
                   size_t size) noexcept override {
            fwrite(data, sizeof(char), size, stderr);
        }


        void flush() noexcept override { }
        void close() noexcept override { }
        void prologue(const char*, size_t) noexcept override { }
        void epilogue(const char*, size_t) noexcept override { }

    };   // conerr


}   // namespace chronicle::sinks
