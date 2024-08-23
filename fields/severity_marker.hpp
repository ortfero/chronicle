// This file is part of chronicle library
// Copyright 2020-2022 Andrei Ilin <ortfero@gmail.com>
// SPDX-License-Identifier: MIT

#pragma once


#include <ufmt/text.hpp>

#include <chronicle/message.hpp>


namespace chronicle::fields {

    class severity_marker {
    public:
	
        template<typename S, typename D, class TimePoint>
        void print(message<D, TimePoint> const& m,
                    ufmt::basic_text<S>& texter) {
            switch(m.severity) {
            case chronicle::severity::failure: texter << '[' << 'F' << ']'; return;
            case chronicle::severity::error: texter << '[' << 'E' << ']'; return;
            case chronicle::severity::warning: texter << '[' << 'W' << ']'; return;
            case chronicle::severity::info: texter << ' ' << ' ' << ' '; return;
            case chronicle::severity::extra: texter << ' ' << ' ' << ' '; return;
            case chronicle::severity::trace: texter << '[' << 'T' << ']'; return;
            case chronicle::severity::debug: texter << '[' << 'D' << ']'; return;
            }
        }

    };   // severity_marker

}   // namespace chronicle::fields
