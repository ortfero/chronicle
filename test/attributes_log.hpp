#pragma once


#include "doctest.h"
#include <chronicle/attributes_log.hpp>
#include <chronicle/sinks/conout.hpp>


TEST_CASE("attributes_log::attributes_log") {
  chronicle::attributes_log target;
  REQUIRE(!target.opened());
  target.info("test", "info", "name", "value"); // should be ok
}


TEST_CASE("attributes_log::attributes_log") {
  chronicle::attributes_log target;
  target.add_sink(chronicle::sinks::conout::open());
  bool const opened = target.open();
  REQUIRE(target.opened());
  target.info("test", "info", "name", "value");
}

