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
        target.add_sink(chronicle::sinks::conout::open());
        bool const opened = target.open();
        REQUIRE(target.opened());
        target.info("test", "info", "name", "value");
    }
}
