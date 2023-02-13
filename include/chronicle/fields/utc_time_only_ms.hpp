// This file is part of chronicle library
// Copyright 2020-2022 Andrei Ilin <ortfero@gmail.com>
// SPDX-License-Identifier: MIT

#pragma once


#include <chrono>

#include <ufmt/text.hpp>

#include <chronicle/message.hpp>


namespace chronicle::fields {


    class utc_time_only_ms {
    public:
        template<class S, typename D, class TimePoint>
        void print(message<D, TimePoint> const& m, ufmt::basic_text<S>& text) {
            namespace chr = std::chrono;
            auto const dp = chr::floor<chr::days>(m.time);
            auto const tod =
                chr::hh_mm_ss {chr::duration_cast<chr::milliseconds>(m.time - dp)};

            if(tod.hours().count() < 10)
                text << '0';
            text << tod.hours().count();
            text << ':';

            if(tod.minutes().count() < 10)
                text << '0';
            text << tod.minutes().count();
            text << ':';

            if(tod.seconds().count() < 10)
                text << '0';
            text << tod.seconds().count();
            text << '.';

            auto const millis = tod.subseconds().count();
            if(millis < 100) {
                if(millis < 10)
                    text << '0' << '0';
                else
                    text << '0';
            }
            text << millis;
        }

    };   // utc_time_only_ms


}   // namespace chronicle::fields
