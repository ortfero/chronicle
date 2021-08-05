#pragma once


#include <system_error>

#include "doctest.h"

#include <chronicle/bundled/date/date.h>
#include <chronicle/data_log.hpp>
#include <chronicle/sinks/conerr.hpp>
#include <chronicle/sinks/conout.hpp>
#include <chronicle/sinks/daily_rotated_file.hpp>
#include <chronicle/sinks/file.hpp>


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
    std::error_code failed;
    auto file = chronicle::sinks::file::open("data.log", failed);
    REQUIRE(!!file);
    REQUIRE(target.add_sink(std::move(file)));
    bool const opened = target.open();
    REQUIRE(opened);
    REQUIRE(target.opened());
    target.close();
    REQUIRE(!target.opened());
    target.info("test", "info", "ok");   // should be ok
    std::filesystem::remove("data.log", failed);
}


TEST_CASE("data_log::notice") {
    chronicle::shared_data_log<std::string> target(256);
    std::error_code failed;
    auto file = chronicle::sinks::file::open("data.log", failed);
    REQUIRE(!!file);
    REQUIRE(target.add_sink(std::move(file)));
    target.open(1);
    auto printer = [&] {
        for(int i = 0; i != 10; ++i)
            target.info("test", "info", "");
    };
    printer();
    target.close();
    std::filesystem::remove("data.log", failed);
}


TEST_CASE("conout") {
    chronicle::shared_data_log<std::string> target(256);
    REQUIRE(target.add_sink(chronicle::sinks::conout::open()));
    bool const opened = target.open();
    REQUIRE(opened);
    target.info("test", "info", "");
}


TEST_CASE("conerr") {
    chronicle::shared_data_log<std::string> target(256);
    REQUIRE(target.add_sink(chronicle::sinks::conerr::open()));
    bool const opened = target.open();
    REQUIRE(opened);
    target.info("test", "info", "");
}
