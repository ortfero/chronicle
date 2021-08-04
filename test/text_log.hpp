#pragma once


#include "doctest.h"

#include <chronicle/sinks/conout.hpp>
#include <chronicle/text_log.hpp>


TEST_CASE("text_log::text_log") {
    chronicle::shared_text_log target;
    REQUIRE(!target.opened());
    target.info("test", "info", " ok");   // should be ok
}


TEST_CASE("text_log::text_log") {
    chronicle::shared_text_log target;
    target.add_sink(chronicle::sinks::conout::open());
    bool const opened = target.open();
    REQUIRE(target.opened());
    target.info("test", "info", " ok");
}
