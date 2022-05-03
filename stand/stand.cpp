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
    auto log = chronicle::unique_data_log<int>{256};
    log.add_sink(std::move(sink));
    if(!log.open())
        return ufmt::error_with(-1, "Unable to open test log");
    ufmt::print("Press ENTER to stop");
    std::getchar();
    log.close();
    //std::filesystem::remove("test.log", ec);
    return 0;
}
