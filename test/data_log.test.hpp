#pragma once


#include "doctest.h"

#include <chronicle/data_log.hpp>
#include <chronicle/sinks/conerr.hpp>
#include <chronicle/sinks/conout.hpp>
#include <chronicle/sinks/daily_rotated_file.hpp>
#include <chronicle/sinks/file.hpp>
#include <date/date.h>


TEST_SUITE("data_log") {

    TEST_CASE("data_log::data_log") {
        chronicle::shared_data_log<std::string> target(256);
        REQUIRE(!target.opened());
        target.info("test", "info", "ok");   // should be ok
    }


    TEST_CASE("data_log::close") {
        chronicle::shared_data_log<std::string> target(256);
        target.close();   // should be ok
    }


    TEST_CASE("data_log::open") {
        chronicle::shared_data_log<std::string> target(256);
        auto const opened = target.open(chronicle::sinks::file::open("data.log"));
        REQUIRE(opened);
        REQUIRE(target.opened());
        target.close();
        REQUIRE(!target.opened());
        target.info("test", "info", "ok");   // should be ok
        std::error_code failed;
        std::filesystem::remove("data.log", failed);
    }


    TEST_CASE("data_log::notice") {
        chronicle::shared_data_log<std::string> target(256);
        auto const opened = target.open(chronicle::sinks::file::open("data.log"), 1);
        REQUIRE(opened);
        auto printer = [&] {
            for(int i = 0; i != 10; ++i)
                target.info("test", "info", "");
        };
        printer();
        target.close();
        std::error_code failed;
        std::filesystem::remove("data.log", failed);
    }


    TEST_CASE("conout") {
        chronicle::shared_data_log<std::string> target(256);
        auto const opened = target.open(chronicle::sinks::conout::open());
        REQUIRE(opened);
        target.info("test", "info", "");
    }


    TEST_CASE("conerr") {
        chronicle::shared_data_log<std::string> target(256);
        auto const opened = target.open(chronicle::sinks::conerr::open());
        REQUIRE(opened);
        target.info("test", "info", "");
    }

}
