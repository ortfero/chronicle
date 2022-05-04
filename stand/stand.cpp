#include <atomic>
#include <chrono>
#include <cstdio>
#include <filesystem>
#include <thread>

#include <chronicle/structured_log.hpp>
#include <chronicle/sinks/file.hpp>
#include <ufmt/print.hpp>


int main(int, char**) {
    auto log = chronicle::shared_structured_log{};
    auto const opened = log.open(chronicle::sinks::file::open("test.log"));
    if(!opened)
        return ufmt::error_with(-1, "Unable to open test log: ", opened.error().message());
    auto done = std::atomic_bool{false};
    auto logger = std::thread([&](){
        while(!done) {
            log.info("stand", "info");
            std::this_thread::sleep_for(std::chrono::milliseconds{100});
        }
    });
    ufmt::print("Press ENTER to stop");
    std::getchar();
    done = true;
    logger.join();
    log.close();
    //std::filesystem::remove("test.log", ec);
    return 0;
}
