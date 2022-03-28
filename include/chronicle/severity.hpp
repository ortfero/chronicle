// This file is part of chronicle library
// Copyright 2020-2022 Andrei Ilin <ortfero@gmail.com>
// SPDX-License-Identifier: MIT

#pragma once


#include <optional>
#include <string>
#include <string_view>


namespace chronicle {


    enum class severity {
        failure,
        error,
        warning,
        info,
        extra,
        trace,
        debug
    };   // severity


    inline std::optional<severity> parse_severity(std::string_view const& text) noexcept {
        if(text.empty())
            return std::nullopt;
        switch(text.size()) {
	case 4: return (text == "info") ? {severity::info} : std::nullopt;
        case 5:
            switch(text[0]) {
            case 'e':
                if(text == "error")
                    return {severity::error};
                if(text == "extra")
                    return {severity::extra};
                return std::nullopt;
            case 't':
                return (text == "trace") ? {severity::trace}
                                         : std::nullopt;
            case 'd':
                return (text == "debug") ? {severity::debug}
                                         : std::nullopt;
	    default: return std::nullopt;
            }
        case 7:
            switch(text[0]) {
            case 'f':
                return (text == "failure") ? {severity::failure}
                                           : std::nullopt;
            case 'w':
                return (text == "warning") ? {severity::warning}
                                           : std::nullopt;
	    default: return std::nullopt;
            }
	default: return std::nullopt;
        }
    }


    inline severity parse_severity(std::string const& text) {
        return parse_severity(std::string_view {text.data(), text.size()});
    }


    inline std::string format(severity s) {
        switch(s) {
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
