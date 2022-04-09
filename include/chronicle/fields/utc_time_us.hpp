// This file is part of chronicle library
// Copyright 2020-2022 Andrei Ilin <ortfero@gmail.com>
// SPDX-License-Identifier: MIT

#pragma once


#include <date/date.h>

#include <ufmt/text.hpp>

#include <chronicle/message.hpp>


namespace chronicle::fields {


    class utc_time_us {
    public:
        template<class S, typename D, class TimePoint>
        void print(message<D, TimePoint> const& m, ufmt::basic_text<S>& text) {
            using namespace std::chrono;
            auto const dp = floor<days>(m.time);
            auto const ymd = date::year_month_day {dp};
            auto const tod = date::time_of_day<microseconds> {
                duration_cast<microseconds>(m.time - dp)};

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

    };   // utc_time_us


}   // namespace chronicle::fields
