// This file is part of chronicle library
// Copyright 2020-2022 Andrei Ilin <ortfero@gmail.com>
// SPDX-License-Identifier: MIT

#pragma once


#include <chrono>

#include <chronicle/severity.hpp>


namespace chronicle {


    class sink {
        severity severity_{severity::info};
    public:
        using time_point = std::chrono::system_clock::time_point;

        sink() noexcept = default;
        sink(severity s) noexcept: severity_{s} { }

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


}   // namespace chronicle
