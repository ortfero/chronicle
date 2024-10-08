﻿// This file is part of chronicle library
// Copyright 2020-2022 Andrei Ilin <ortfero@gmail.com>
// SPDX-License-Identifier: MIT

#pragma once


#include <chrono>

#include <ufmt/text.hpp>

#include <chronicle/message.hpp>


namespace chronicle::fields {

    class utc_time_ms {
    public:
        template<class S, typename D, class TimePoint>
        void print(message<D, TimePoint> const& m, ufmt::basic_text<S>& text) {
            namespace chr =  std::chrono;
            auto const dp = chr::floor<chr::days>(m.time);
            auto const ymd = chr::year_month_day {dp};
            auto const tod = chr::hh_mm_ss<chr::milliseconds> {
                duration_cast<chr::milliseconds>(m.time - dp)};

            text << int(ymd.year()) << '-';
            if(unsigned(ymd.month()) < 10)
                text << '0';
            text << unsigned(ymd.month());
            text << '-';
            if(unsigned(ymd.day()) < 10)
                text << '0';
            text << unsigned(ymd.day());
            text << ' ';

            if(tod.hours().count() < 10)
                text << '0';
            text << int(tod.hours().count());
            text << ':';

            if(tod.minutes().count() < 10)
                text << '0';
            text << int(tod.minutes().count());
            text << ':';

            if(tod.seconds().count() < 10)
                text << '0';
            text << int(tod.seconds().count());
            text << '.';

            auto const millis = int(tod.subseconds().count());
            if(millis < 100) {
                if(millis < 10)
                    text << '0' << '0';
                else
                    text << '0';
            }

            text << int(millis);
        }

    };   // utc_time_ms


}   // namespace chronicle::fields
