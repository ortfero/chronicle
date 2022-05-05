// This file is part of chronicle library
// Copyright 2020-2022 Andrei Ilin <ortfero@gmail.com>
// SPDX-License-Identifier: MIT

#pragma once


#include <tuple>

#include <ufmt/text.hpp>

#include <chronicle/message.hpp>


namespace chronicle::fields {


    template<class... Fields>
    class format {
    public:
        using fields_type = std::tuple<Fields...>;


        template<class S, typename D, class TimePoint>
        void print(message<D, TimePoint> const& m, ufmt::basic_text<S>& text) {
            print_fields<S, D, TimePoint, 0>(m, text);
            text << ' ';
            text << m.text;

            if(m.has_data)
                text << m.data;

            text << '\n';
        }


    private:
        fields_type fields_;

        template<class S, typename D, class TimePoint, size_t I>
        void print_fields(message<D, TimePoint> const& message,
                          ufmt::basic_text<S>& text) {
            std::get<I>(fields_).print(message, text);

            if constexpr(I + 1 != std::tuple_size_v<fields_type>) {
                text << ' ';
                print_fields<S, D, TimePoint, I + 1>(message, text);
            }
        }

    };   // format


}   // namespace chronicle::fields
