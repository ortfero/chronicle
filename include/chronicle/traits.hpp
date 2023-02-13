// This file is part of chronicle library
// Copyright 2020-2022 Andrei Ilin <ortfero@gmail.com>
// SPDX-License-Identifier: MIT

#pragma once


#include <chrono>

#include <hydra/mpsc_queue.hpp>
#include <hydra/spsc_queue.hpp>

#include <chronicle/fields/default_format.hpp>
#include <chronicle/message.hpp>


namespace chronicle {

    template<typename D>
    class default_data_formatter {
    public:

        template<typename S> static void format(S& stream,
                                                D const& data) {
            stream << data;
        }

    }; // default_data_formatter


    template<typename D, class Q, class F, class C, class DF>
    struct basic_traits {
        using data_type = D;
        using queue_type = Q;
        using format_type = F;
        using clock_type = C;
        using data_formatter_type = DF;
    };   // basic_traits


    template<typename D, class F, class C, class DF = default_data_formatter<D>>
    using traits_unique =
        basic_traits<D,
                     hydra::spsc_queue<message<D, typename C::time_point>>,
                     F,
                     C,
                     DF>;

    template<typename D, class F, class C, class DF = default_data_formatter<D>>
    using traits_shared =
        basic_traits<D,
                     hydra::mpsc_queue<message<D, typename C::time_point>>,
                     F,
                     C,
                     DF>;

    template<typename D, class C = std::chrono::system_clock, class DF = default_data_formatter<D>>
    using traits_unique_default =
        traits_unique<D, fields::format_singlethreaded_default, C, DF>;

    template<typename D, class C = std::chrono::system_clock, class DF = default_data_formatter<D>>
    using traits_shared_default =
        traits_shared<D, fields::format_multithreaded_default, C, DF>;

    template<typename D, class C = std::chrono::system_clock, class DF = default_data_formatter<D>>
    using traits_unique_ms =
        traits_unique<D, fields::format_singlethreaded_ms, C, DF>;

    template<typename D, class C = std::chrono::system_clock, class DF = default_data_formatter<D>>
    using traits_shared_ms =
        traits_shared<D, fields::format_multithreaded_ms, C, DF>;

    template<typename D, class C = std::chrono::system_clock, class DF = default_data_formatter<D>>
    using traits_unique_us =
        traits_unique<D, fields::format_singlethreaded_us, C, DF>;

    template<typename D, class C = std::chrono::system_clock, class DF = default_data_formatter<D>>
    using traits_shared_us =
        traits_shared<D, fields::format_multithreaded_us, C, DF>;

    template<typename D, class C = std::chrono::system_clock, class DF = default_data_formatter<D>>
    using traits_unique_time_only_ms =
        traits_unique<D, fields::format_singlethreaded_time_only_ms, C, DF>;

    template<typename D, class C = std::chrono::system_clock, class DF = default_data_formatter<D>>
    using traits_shared_time_only_ms =
        traits_unique<D, fields::format_multithreaded_time_only_ms, C, DF>;

    template<typename D, class C = std::chrono::system_clock, class DF = default_data_formatter<D>>
    using traits_unique_time_only_us =
        traits_unique<D, fields::format_singlethreaded_time_only_us, C, DF>;

    template<typename D, class C = std::chrono::system_clock, class DF = default_data_formatter<D>>
    using traits_shared_time_only_us =
        traits_shared<D, fields::format_multithreaded_time_only_us, C, DF>;


}   // namespace chronicle
