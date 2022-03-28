// This file is part of chronicle library
// Copyright 2020-2022 Andrei Ilin <ortfero@gmail.com>
// SPDX-License-Identifier: MIT

#pragma once


#include <string>
#include <string_view>


namespace chronicle {


    enum class severity {
        undefined,
        failure,
        error,
        warning,
        info,
        extra,
        trace,
        debug
    };   // severity


    inline severity parse_severity(std::string_view const& text) {
        if(text.empty())
            return severity::undefined;
        switch(text.size()) {
        case 4: return (text == "info") ? severity::info : severity::undefined;
        case 5:
            switch(text[0]) {
            case 'e':
                if(text == "error")
                    return severity::error;
                if(text == "extra")
                    return severity::extra;
                return severity::undefined;
            case 't':
                return (text == "trace") ? severity::trace
                                         : severity::undefined;
            case 'd':
                return (text == "debug") ? severity::debug
                                         : severity::undefined;
            default: return severity::undefined;
            }
        case 7:
            switch(text[0]) {
            case 'f':
                return (text == "failure") ? severity::failure
                                           : severity::undefined;
            case 'w':
                return (text == "warning") ? severity::warning
                                           : severity::undefined;
            default: return severity::undefined;
            }
        default: return severity::undefined;
        }
    }


    inline severity parse_severity(std::string const& text) {
        return parse_severity(std::string_view {text.data(), text.size()});
    }


    inline std::string format(severity s) {
        switch(s) {
        case severity::undefined: return std::string("undefined");
        case severity::failure: return std::string("failure");
        case severity::error: return std::string("error");
        case severity::warning: return std::string("warning");
        case severity::info: return std::string("info");
        case severity::extra: return std::string("extra");
        case severity::trace: return std::string("trace");
        case severity::debug: return std::string("debug");
        default: return std::string("unknown");
        }
    }


    template<typename S>
    S& operator<<(S& stream, severity s) {
        return stream << format(s);
    }


}   // namespace chronicle
