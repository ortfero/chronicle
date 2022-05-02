#include <chrono>
#include <cstdio>
#include <filesystem>
#include <thread>

#include <chronicle/structured_log.hpp>
#include <chronicle/sinks/file.hpp>
#include <ufmt/print.hpp>


int main(int, char**) {
    auto ec = std::error_code{};
    auto sink = chronicle::sinks::file::open("test.log", ec);
    if(!sink)
        return ufmt::error_with(-1, "Unable to open test log: ", ec.message());
    auto log = chronicle::shared_structured_log{};
    log.add_sink(std::move(sink));
    if(!log.open())
        return ufmt::error_with(-1, "Unable to open test log");
    auto done = std::atomic<bool>{false};
    auto logger = std::thread{[&]() {
        while(!done) {
            //log.info("tag", "info");
            std::this_thread::sleep_for(std::chrono::milliseconds{100});
        }
    }};
    ufmt::print("Press ENTER to stop");
    std::getchar();
    done = true;
    std::filesystem::remove("test.log", ec);
    logger.join();
    return 0;
}
