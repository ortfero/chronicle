#pragma once


#include "doctest.h"
#include <chronicle/sinks/conout.hpp>
#include <chronicle/structured_log.hpp>

TEST_SUITE("structuted_log") {
    TEST_CASE("structured_log::structured_log") {
        chronicle::shared_structured_log target;
        REQUIRE(!target.opened());
        target.info("test", "info", "name", "value");   // should be ok
    }


    TEST_CASE("structured_log::attributes_log") {
        chronicle::shared_structured_log target;
        auto const opened = target.open(chronicle::sinks::conout::open());
        REQUIRE(opened);
        target.info("test", "info", "name", "value");
    }
}
