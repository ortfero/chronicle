#define _CRT_SECURE_NO_WARNINGS

#include <cstdio>
#include <charconv>
#include <iostream>
#include <chrono>
#include <vector>


#include <ubench/ubench.hpp>

#include <chronicle/text_log.hpp>

#include <reckless/file_writer.hpp>
#include <reckless/policy_log.hpp>

#include <nanolog/NanoLog.hpp>

#include <spdlog/spdlog.h>
#include <spdlog/async.h>
#include <spdlog/sinks/basic_file_sink.h>

#include <ubench/ubench.hpp>

struct settings {
  static constexpr auto total_messages = 100000;
  static constexpr auto int_number = 127562;
  static constexpr auto string = "benchmark";
  static constexpr auto float_number = 3.14;
  static constexpr auto character = 'K';
};


chronicle::file_text_log<> journal;

reckless::file_writer reckless_writer("reckless.log");
using reckless_logger_t = reckless::policy_log<>;
reckless_logger_t reckless_logger(&reckless_writer);

std::shared_ptr<spdlog::logger>  spd_logger;



template<typename F>
void run_benchmark(char const* name, int thread_count, F&& f) {

  std::cout << "Thread count: " << thread_count << std::endl;
  std::vector<std::thread> threads;

  auto benchmark = [&]() {

    using namespace std::chrono;
    const auto iterations = settings::total_messages / thread_count;
    auto const begin = steady_clock::now();
    for(int i = 0; i != iterations; ++i)
      f();
    auto const end = steady_clock::now();
    auto const latency = duration_cast<nanoseconds>((end - begin) / iterations);
    std::cout << "Average " << name << " latency - " << latency.count() << " nanoseconds" << std::endl;
    std::cout << name << " to save " << settings::total_messages << " messages - "
              << duration_cast<milliseconds>(end - begin).count() << " ms" << std::endl;

  };

  for(int i = 0; i != thread_count; ++i)
    threads.emplace_back(benchmark);

  for(int i = 0; i != thread_count; ++i)
    threads[i].join();

}


int main() {

  using namespace  std;

  if(!journal.open(chronicle::sinks::file::open("test.log"), 1000000)) {
    puts("Unable to open log");
    return 1;
  }

  nanolog::initialize(nanolog::GuaranteedLogger(), "", "nanolog", 1);

  for (auto threads : { 1 })
    run_benchmark("nanolog", threads, [] {
    LOG_INFO << "Logging "
      << settings::string << settings::int_number
      << settings::character << settings::float_number;
      });
  

  spdlog::init_thread_pool(8192, 1);
  spd_logger = spdlog::basic_logger_mt<spdlog::async_factory>(
                 "spdlog", "spd.log", true);
  spd_logger->set_pattern("[%l] %v");


  for(auto threads: {1})
   run_benchmark("reckless", threads, []{
      reckless_logger.write("[benchmark] Logging {} {}{}{}",
                            settings::string, settings::int_number,
                            settings::character, settings::float_number);
    });

  for(auto threads: {1})
   run_benchmark("spdlog", threads, []{
      spd_logger->info("Logging {} {}{}{}",
                            settings::string, settings::int_number,
                            settings::character, settings::float_number);
    });


  for(auto threads: {1})
    run_benchmark("chronicle", threads, []{
      journal.notice("benchmark", "Logging ",
                     settings::string/*, settings::int_number*/,
                     settings::character/*, settings::float_number*/);
    });

  std::cout << "chronicle blocks: " << journal.blocks_count() << std::endl;

  return 0;

}
