#include <charconv>
#include <chrono>
#include <cstdio>
#include <iostream>
#include <vector>


#include "ubench.hpp"

#include <chronicle/sinks/file.hpp>
#include <chronicle/text_log.hpp>

#include <NanoLog.hpp>

#include <spdlog/async.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/spdlog.h>


struct settings {
    static constexpr auto total_messages = 100000;
    static constexpr auto int_number = 127562;
    static constexpr auto string = std::string_view {"benchmark"};
    static constexpr auto float_number = 3.14;
    static constexpr auto character = 'K';
};


chronicle::shared_text_log chronicle_logger;
std::shared_ptr<spdlog::logger> spd_logger;



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
        auto const latency =
            duration_cast<nanoseconds>((end - begin) / iterations);
        std::cout << "Average " << name << " latency - " << latency.count()
                  << " nanoseconds" << std::endl;
    };

    for(int i = 0; i != thread_count; ++i)
        threads.emplace_back(benchmark);

    for(int i = 0; i != thread_count; ++i)
        threads[i].join();
}


int main() {
    using namespace std;

    if(!chronicle_logger.open(chronicle::sinks::file::open("test.log"), 128 * 1024)) {
        puts("Unable to open log");
        return 1;
    }

    nanolog::initialize(nanolog::GuaranteedLogger(), "", "nanolog", 1);

    for(auto threads: {2})
        run_benchmark("nanolog", threads, [] {
            LOG_INFO << "Logging " << settings::string.data()
                     <<' ' << settings::int_number
                     << ' ' << settings::float_number;
        });


    spdlog::init_thread_pool(128 * 1024, 1);
    spd_logger = spdlog::basic_logger_mt<spdlog::async_factory>("spdlog",
                                                                "spd.log",
                                                                true);
    spd_logger->set_pattern("[%l] %v");


    for(auto threads: {2})
        run_benchmark("spdlog", threads, [] {
            spd_logger->info("Logging {} {} {}",
                             settings::string,
                             settings::int_number,
                             settings::float_number);
        });


    for(auto threads: {2})
        run_benchmark("chronicle", threads, [] {
            chronicle_logger.info("benchmark",
                "Logging ", settings::string,
                ' ', settings::int_number,
                ' ', settings::float_number);
        });

    std::cout << "chronicle blocks: " << chronicle_logger.blocks_count()
              << std::endl;

    return 0;
}
