// This file is part of chronicle library
// Copyright 2020-2022 Andrei Ilin <ortfero@gmail.com>
// SPDX-License-Identifier: MIT

#pragma once


#include <ufmt/text.hpp>

#include <chronicle/message.hpp>


namespace chronicle::fields {

    class utc_time_only_us {
    public:
        template<class S, typename D, class TimePoint>
        void print(message<D, TimePoint> const& m, ufmt::basic_text<S>& text) {
            namespace chr = std::chrono;
            auto const dp = chr::floor<chr::days>(m.time);
            auto const tod =
                chr::hh_mm_ss {chr::duration_cast<chr::microseconds>(m.time - dp)};

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

            auto const micros = tod.subseconds().count();
            text << ufmt::fixed(micros, 6);
        }

    };   // utc_time_only_us


}   // namespace chronicle::fields
