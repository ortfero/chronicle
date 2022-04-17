// This file is part of chronicle library
// Copyright 2020-2021 Andrei Ilin <ortfero@gmail.com>
// SPDX-License-Identifier: MIT

#pragma once


#include <ufmt/text.hpp>

#include <chronicle/message.hpp>


namespace chronicle::fields {


    class severity {
    public:
        template<class S, typename D, class TimePoint>
        void print(message<D, TimePoint> const& m, ufmt::basic_text<S>& text) {
            switch(m.severity) {
            case chronicle::severity::failure: text << "failure"; break;
            case chronicle::severity::error: text << "error  "; break;
            case chronicle::severity::warning: text << "warning"; break;
            case chronicle::severity::info: text << "       "; break;
            case chronicle::severity::extra: text << "       "; break;
            case chronicle::severity::trace: text << "trace  "; break;
            case chronicle::severity::debug: text << "debug  "; break;
            default: text << "unknown"; break;
            }
        }

    };   // severity


}   // namespace chronicle::fields
