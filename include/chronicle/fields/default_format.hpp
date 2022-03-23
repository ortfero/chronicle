// This file is part of chronicle library
// Copyright 2020-2022 Andrei Ilin <ortfero@gmail.com>
// SPDX-License-Identifier: MIT

#pragma once


#include "format.hpp"
#include "severity.hpp"
#include "utc_time_us.hpp"
#include "utc_time_ms.hpp"
#include "utc_time_only_us.hpp"
#include "utc_time_only_ms.hpp"
#include "thread_id.hpp"
#include "source.hpp"


namespace chronicle::fields {
  
  
  using format_multithreaded_us = format<severity, utc_time_us, thread_id, source>;
  using format_multithreaded_ms = format<severity, utc_time_ms, thread_id, source>;
  using format_multithreaded_time_only_us = format<severity, utc_time_only_us, thread_id, source>;
  using format_multithreaded_time_only_ms = format<severity, utc_time_only_ms, thread_id, source>;
  
  using format_singlethreaded_us = format<severity, utc_time_us, source>;
  using format_singlethreaded_ms = format<severity, utc_time_ms, source>;

  using format_singlethreaded_time_only_us = format<severity, utc_time_only_us, source>;
  using format_singlethreaded_time_only_ms = format<severity, utc_time_only_ms, source>;
  
  using format_multithreaded_default = format_multithreaded_us;
  using format_singlethreaded_default = format_singlethreaded_us;
  
  
} // namespace chronicle::fields
