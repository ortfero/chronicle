#pragma once


#include "doctest.h"

#include <chronicle/sinks/conout.hpp>
#include <chronicle/text_log.hpp>

TEST_SUITE("text_log") {

    TEST_CASE("info for closed log") {
        chronicle::shared_text_log target;
        REQUIRE(!target.opened());
        target.info("test", "info", " ok");   // should be ok
    }


    TEST_CASE("info to terminal") {
        chronicle::shared_text_log target;
        target.add_sink(chronicle::sinks::conout::open());
        target.open();
        REQUIRE(target.opened());
        target.info("test", "info", " ok");
    }


    TEST_CASE("uint64_t to terminal") {
        chronicle::shared_text_log target;
        target.add_sink(chronicle::sinks::conout::open());
        target.open();
        auto const n = 0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF;
        target.info("test", n);
    }
}
