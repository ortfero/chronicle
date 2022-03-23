// This file is part of chronicle library
// Copyright 2020-2022 Andrei Ilin <ortfero@gmail.com>
// SPDX-License-Identifier: MIT

#pragma once


#include <string_view>
#include <chrono>


#ifdef CHRONICLE_USE_SYSTEM_HYDRA

#include <hydra/sequence.hpp>

#else

#include "bundled/hydra/sequence.hpp"

#endif // CHRONICLE_USE_SYSTEM_HYDRA


#include "severity.hpp"


namespace chronicle {


  template<typename D, class TimePoint> struct message {

    hydra::sequence sequence;
    enum severity severity;
    TimePoint time;
    unsigned thread_id;
    std::string_view source;
    std::string_view text;
    bool has_data{false};
    D data;

  }; // message


} // chronicle
