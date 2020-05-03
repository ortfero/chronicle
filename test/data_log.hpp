#pragma once


#include <doctest/doctest.h>
#include <string>
#include <chronicle/data_log.hpp>
#include <chronicle/detail/chineseroom/texter.hpp>
#include <ubench/ubench.hpp>


TEST_CASE("data_log::data_log") {

  //chronicle::file_data_log<std::string> target(256);
  //REQUIRE(!target.opened());
  //target.notice("test", "info", "ok"); // should be ok
}


TEST_CASE("data_log::close") {

  //chronicle::file_data_log<std::string> target(256);
  //target.close(); // should be ok
}


TEST_CASE("data_log::open") {

  //chronicle::file_data_log<std::string> target(256);
  //bool const opened = target.open(chronicle::sinks::file::open("data.log"));
  //REQUIRE(opened);
  //REQUIRE(target.opened());
  //target.close();
  //REQUIRE(!target.opened());
  //target.notice("test", "info", "ok"); // should be ok
}




TEST_CASE("data_log::notice") {
  chronicle::file_data_log<std::string> target(256);
  target.open(chronicle::sinks::file::open("data.log"), 1);
  auto printer = [&] {
    for(int i = 0; i != 10; ++i)
      target.notice("test", "info", "");
  };
  printer();
}






TEST_CASE("Time label") {

  auto const sc = ubench::run([]{ std::chrono::system_clock::now(); });
  auto const ft = ubench::run([]{ FILETIME ft; ::GetSystemTimePreciseAsFileTime(&ft); });
  printf("std::chrono::system_clock::now = %.2f ns\n"
         "GetSystemTimePreciseAsFileTime = %.2f ns\n", sc.time.count(), ft.time.count());

}


TEST_CASE("Formatting output") {

  chineseroom::std_texter t;

  t.print("pi = ").fixed(123, 5);
  REQUIRE(t.string() == "pi = 00123");

}
