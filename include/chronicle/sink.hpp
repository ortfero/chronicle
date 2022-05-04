// This file is part of chronicle library
// Copyright 2020-2022 Andrei Ilin <ortfero@gmail.com>
// SPDX-License-Identifier: MIT

#pragma once


#include <chrono>
#include <memory>
#include <system_error>

#include <tl/expected.hpp>

#include <chronicle/severity.hpp>


namespace chronicle {


    class sink {
        enum severity severity_{severity::info};
        
    public:
        using time_point = std::chrono::system_clock::time_point;

        sink() noexcept = default;
        sink(severity s) noexcept: severity_{s} { }
        
        enum severity severity() const noexcept { return severity_; }
        void severity(enum severity s) noexcept { severity_ = s; }

        virtual bool ready() const noexcept = 0;

        virtual void write(time_point const& tp,
                           char const* data,
                           size_t size) noexcept = 0;

        virtual void flush() noexcept = 0;

        virtual void close() noexcept = 0;

        virtual void prologue(char const* data, size_t size) noexcept = 0;

        virtual void epilogue(char const* data, size_t size) noexcept = 0;

        virtual ~sink() {}

    };   // sink
    
    using sink_ptr = std::unique_ptr<sink>;
    using expected_sink_ptr = tl::expected<sink_ptr, std::error_code>;


}   // namespace chronicle
