#pragma once


#include <chrono>

#include <chronicle/sinks/daily_rotated_file.hpp>
#include <date/date.h>

#include "doctest.h"


TEST_SUITE("daily_rotated_file") {
    TEST_CASE("daily_rotated_file::daily_rotated_file()") {
        chronicle::sinks::daily_rotated_file target;
        REQUIRE(!target.ready());
    }


    TEST_CASE("daily_rotated_file::open") {
        std::error_code ec;
        auto target =
            chronicle::sinks::daily_rotated_file::open("test.log", ec);
        REQUIRE(!!target);
        REQUIRE(target->ready());
    }


    TEST_CASE("daily_rotated_file::file_path") {
        std::error_code ec;
        auto target =
            chronicle::sinks::daily_rotated_file::open("test.log", ec);
        REQUIRE(!!target);
        REQUIRE(target->ready());
        using namespace std::chrono;
        auto const now = system_clock::now();
        auto const dp = floor<date::days>(now);
        date::year_month_day const ymd = dp;
        char file_path[256];
        snprintf(file_path,
                 sizeof(file_path),
                 "test-%d_%02u_%02u.log",
                 int(ymd.year()),
                 unsigned(ymd.month()),
                 unsigned(ymd.day()));
        REQUIRE(target->file_path().string() == file_path);
    }


    TEST_CASE("daily_rotated_file::rotate_file") {
        std::error_code ec;
        auto target =
            chronicle::sinks::daily_rotated_file::open("test.log", ec);
        REQUIRE(!!target);
        REQUIRE(target->ready());
        using namespace std::chrono;
        auto const tomorrow = system_clock::now() + hours {24};
        target->write(tomorrow, "test", sizeof("test") - 1);
        auto const dp = floor<date::days>(tomorrow);
        date::year_month_day const ymd = dp;
        char file_path[256];
        snprintf(file_path,
                 sizeof(file_path),
                 "test-%d_%02u_%02u.log",
                 int(ymd.year()),
                 unsigned(ymd.month()),
                 unsigned(ymd.day()));
        REQUIRE(target->file_path().string() == file_path);
    }
}
