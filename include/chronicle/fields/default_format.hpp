// This file is part of chronicle library
// Copyright 2020-2022 Andrei Ilin <ortfero@gmail.com>
// SPDX-License-Identifier: MIT

#pragma once


#include <chronicle/fields/format.hpp>
#include <chronicle/fields/severity_marker.hpp>
#include <chronicle/fields/source.hpp>
#include <chronicle/fields/thread_id.hpp>
#include <chronicle/fields/utc_time_ms.hpp>
#include <chronicle/fields/utc_time_only_ms.hpp>
#include <chronicle/fields/utc_time_only_us.hpp>
#include <chronicle/fields/utc_time_us.hpp>


namespace chronicle::fields {


    using format_multithreaded_us =
        format<severity_marker, utc_time_ms, thread_id, source>;
    using format_multithreaded_ms =
        format<severity_marker, utc_time_ms, thread_id, source>;
    using format_multithreaded_time_only_us =
        format<severity_marker, utc_time_only_us, thread_id, source>;
    using format_multithreaded_time_only_ms =
        format<severity_marker, utc_time_only_ms, thread_id, source>;

    using format_singlethreaded_us = format<severity_marker, utc_time_us, source>;
    using format_singlethreaded_ms = format<severity_marker, utc_time_ms, source>;

    using format_singlethreaded_time_only_us =
        format<severity_marker, utc_time_only_us, source>;
    using format_singlethreaded_time_only_ms =
        format<severity_marker, utc_time_only_ms, source>;

    using format_multithreaded_default = format_multithreaded_us;
    using format_singlethreaded_default = format_singlethreaded_us;


}   // namespace chronicle::fields
