// This file is part of chronicle library
// Copyright 2020-2022 Andrei Ilin <ortfero@gmail.com>
// SPDX-License-Identifier: MIT

#pragma once


#include <chrono>
#include <memory>
#include <system_error>

#include <etceteras/expected.hpp>
#include <ufmt/text.hpp>

#include <chronicle/severity.hpp>


namespace chronicle {


    class sink {
    public:
        using time_point = std::chrono::system_clock::time_point;
        using size_type = std::size_t;

        virtual bool ready() const noexcept = 0;

        virtual void write(time_point const& tp,
                           char const* data,
                           size_type size) noexcept = 0;

        virtual void flush() noexcept = 0;

        virtual void close() noexcept = 0;

        virtual void prologue(char const* data, size_t size) noexcept = 0;

        virtual void epilogue(char const* data, size_t size) noexcept = 0;

        virtual ~sink() {}

    };   // sink
    
    using sink_ptr = std::unique_ptr<sink>;
    using expected_sink_ptr = etceteras::expected<sink_ptr, std::error_code>;


}   // namespace chronicle
