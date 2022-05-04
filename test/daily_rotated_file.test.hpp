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
        auto expected_target =
            chronicle::sinks::daily_rotated_file::open("test.log");
        REQUIRE(!!expected_target);
        REQUIRE((*expected_target)->ready());
    }


}
