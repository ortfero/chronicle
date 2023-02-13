#include <atomic>
#include <chrono>
#include <cstdio>
#include <filesystem>
#include <thread>

#include <chronicle/data_log.hpp>
#include <chronicle/sinks/file.hpp>
#include <chronicle/fields/severity_marker.hpp>

#include <ufmt/print.hpp>


enum class message_kind {
    none, data
}; // message_kind


union message_data {
    int value;
    
    message_data() { }
    message_data(int v) noexcept: value{v} { }
}; // message_data


struct message {        
    using kind_type = message_kind;

    message_kind kind{message_kind::none};
    message_data data;
    
    message() = default;
    
    message(int value) noexcept
    : kind{message_kind::data}, data{value} { }
    
    template<typename S> friend S& operator << (S& stream, message const& m) {
        switch(m.kind) {
            case kind_type::data:
                return stream << ' ' << '{' << m.data.value << '}';
            default:
                return stream << "#(unknown_message)";
        }
    }
    
}; // message

using format = chronicle::fields::format<
            chronicle::fields::utc_time_ms,
            chronicle::fields::thread_id,
            chronicle::fields::source>;

using traits = chronicle::traits_shared<message, format, std::chrono::system_clock>;
using logger = chronicle::data_log<traits>;


int main(int, char**) {
    auto log = logger{4096};
    auto const opened = log.open(chronicle::sinks::file::open("test.log"));
    if(!opened)
        return ufmt::error_with(-1, "error: ", opened.error().message());
    
    log.info("main", "sample", message{-1}); 
    
    return 0;
}
