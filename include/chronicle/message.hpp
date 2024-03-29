// This file is part of chronicle library
// Copyright 2020-2022 Andrei Ilin <ortfero@gmail.com>
// SPDX-License-Identifier: MIT

#pragma once


#include <chrono>
#include <string_view>

#include <hydra/sequence.hpp>

#include <chronicle/severity.hpp>


namespace chronicle {


    template<typename D, class TimePoint>
    struct message {
        hydra::sequence sequence;
        enum severity severity;
        TimePoint time;
        unsigned thread_id;
        std::string_view source;
        std::string_view text;
        bool has_data {false};
        D data;

    };   // message


}   // namespace chronicle
