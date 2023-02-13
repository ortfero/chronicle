// This file is part of chronicle library
// Copyright 2020-2022 Andrei Ilin <ortfero@gmail.com>
// SPDX-License-Identifier: MIT

#pragma once


#include <atomic>
#include <initializer_list>
#include <memory>
#include <thread>
#include <vector>

#include <etceteras/expected.hpp>


#if defined(_WIN32)

#    if !defined(_X86_) && !defined(_AMD64_) && !defined(_ARM_) \
        && !defined(_ARM64_)
#        if defined(_M_IX86)
#            define _X86_
#        elif defined(_M_AMD64)
#            define _AMD64_
#        elif defined(_M_ARM)
#            define _ARM_
#        elif defined(_M_ARM64)
#            define _ARM64_
#        endif
#    endif

#    include <processthreadsapi.h>

#elif defined(__linux__)

#    include <unistd.h>

#else

#    error Unsupported system

#endif


#include <hydra/activity.hpp>
#include <hydra/batch.hpp>
#include <hydra/mpsc_queue.hpp>
#include <hydra/spsc_queue.hpp>
#include <ufmt/text.hpp>

#include <chronicle/message.hpp>
#include <chronicle/severity.hpp>
#include <chronicle/sink.hpp>
#include <chronicle/traits.hpp>


namespace chronicle {


    template<typename Tr>
    class data_log {
    public:
        using data_type = typename Tr::data_type;
        using format_type = typename Tr::format_type;
        using queue_type = typename Tr::queue_type;
        using clock_type = typename Tr::clock_type;
        using data_formatter_type = typename Tr::data_formatter_type;
        using duration = typename clock_type::duration;
        using time_point = typename clock_type::time_point;
        using message_type = message<data_type, time_point>;
        using activity_type = hydra::activity<message_type, queue_type>;
        using batch_type = typename activity_type::batch_type;
        using size_type = typename activity_type::size_type;
        using sinks_type = std::vector<std::unique_ptr<sink>>;


        static constexpr size_type default_queue_size = 8192;

    private:
        sink_ptr sink_ptr_;
        enum severity severity_ { chronicle::severity::info };
        activity_type activity_;
        size_type message_size_;
        ufmt::text buffer_;
        format_type format_;
        std::string prologue_ {"\n    ++++ log opened ++++\n"};
        std::string epilogue_ {"    ++++ log closed ++++\n\n"};

    public:
        data_log(size_type message_size) noexcept
            : message_size_ {message_size} {}

        data_log(data_log const&) = delete;
        data_log& operator=(data_log const&) = delete;
        ~data_log() { close(); }
        bool opened() const noexcept { return activity_.active(); }
        enum severity severity() const noexcept { return severity_; }
        void severity(enum severity s) noexcept { severity_ = s; }
        
        
        size_type blocks_count() const noexcept {
            return activity_.blocks_count();
        }
        
        
        void prologue(std::string text) noexcept {
            prologue_ = std::move(text);
        }
        
        
        void epilogue(std::string text) noexcept {
            epilogue_ = std::move(text);
        }


        void flush() noexcept {
            if(!sink_ptr_)
                return;
            sink_ptr_->flush();
        }


        etceteras::expected<void, std::error_code>
        open(expected_sink_ptr&& esp, size_type queue_size = default_queue_size) {
            if(!esp)
                return etceteras::make_unexpected(esp.error());
            if(!(*esp)->ready())
                return etceteras::make_unexpected(
                    std::make_error_code(std::errc::bad_file_descriptor));
            sink_ptr_ = std::move(*esp);

            if(!prologue_.empty())
                sink_ptr_->prologue(prologue_.data(), prologue_.size());

            activity_.reserve(queue_size);

            auto const started = activity_.run([this](auto& batch) {
                buffer_.clear();
                buffer_.reserve(message_size_ * batch.size());

                auto const now = clock_type::now();

                while(auto sequence = batch.try_fetch()) {
                    message_type& message = batch[sequence];
                    message.time = now;
                    format_.template print<data_formatter_type>(message, buffer_);
                    batch.fetched();
                }

                sink_ptr_->write(now, buffer_.data(), buffer_.size());
            });

            if(!started)
                return etceteras::make_unexpected(
                    std::make_error_code(std::errc::no_child_process));

            return {};
        }


        void close() {
            if(!activity_.active())
                return;
            activity_.stop();
            if(!epilogue_.empty())
                sink_ptr_->epilogue(epilogue_.data(), epilogue_.size());
            sink_ptr_->close();
        }


        template<size_t N1, size_t N2>
        void failure(char const (&tag)[N1], char const (&text)[N2]) {
            this->template print<severity::failure>(
                std::string_view {tag, N1 - 1},
                std::string_view {text, N2 - 1});
        }


        template<size_t N1, size_t N2>
        void failure(char const (&tag)[N1],
                     char const (&text)[N2],
                     data_type const& data) {
            this->template print<severity::failure>(
                std::string_view {tag, N1 - 1},
                std::string_view {text, N2 - 1},
                data);
        }


        template<size_t N1, size_t N2>
        void error(char const (&tag)[N1], char const (&text)[N2]) {
            this->template print<severity::error>(
                std::string_view {tag, N1 - 1},
                std::string_view {text, N2 - 1});
        }


        template<size_t N1, size_t N2>
        void error(char const (&tag)[N1],
                   char const (&text)[N2],
                   data_type const& data) {
            this->template print<severity::error>(
                std::string_view {tag, N1 - 1},
                std::string_view {text, N2 - 1},
                data);
        }


        template<typename R, size_t N1, size_t N2>
        R error_with(R&& r, char const (&tag)[N1], char const (&text)[N2]) {
            this->template print<severity::error>(
                std::string_view {tag, N1 - 1},
                std::string_view {text, N2 - 1});
            return std::forward<R>(r);
        }


        template<typename R, size_t N1, size_t N2>
        R error_with(R&& r,
                     char const (&tag)[N1],
                     char const (&text)[N2],
                     data_type const& data) {
            this->template print<severity::error>(
                std::string_view {tag, N1 - 1},
                std::string_view {text, N2 - 1},
                data);
            return std::forward<R>(r);
        }



        template<size_t N1, size_t N2>
        void warning(char const (&tag)[N1], char const (&text)[N2]) {
            this->template print<severity::warning>(
                std::string_view {tag, N1 - 1},
                std::string_view {text, N2 - 1});
        }


        template<size_t N1, size_t N2>
        void warning(char const (&tag)[N1],
                     char const (&text)[N2],
                     data_type const& data) {
            this->template print<severity::warning>(
                std::string_view {tag, N1 - 1},
                std::string_view {text, N2 - 1},
                data);
        }


        template<size_t N1, size_t N2>
        void info(char const (&tag)[N1], char const (&text)[N2]) {
            this->template print<severity::info>(
                std::string_view {tag, N1 - 1},
                std::string_view {text, N2 - 1});
        }


        template<size_t N1, size_t N2>
        void info(char const (&tag)[N1],
                  char const (&text)[N2],
                  data_type const& data) {
            this->template print<severity::info>(
                std::string_view {tag, N1 - 1},
                std::string_view {text, N2 - 1},
                data);
        }


        template<size_t N1, size_t N2>
        void extra(char const (&tag)[N1], char const (&text)[N2]) {
            this->template print<severity::extra>(
                std::string_view {tag, N1 - 1},
                std::string_view {text, N2 - 1});
        }


        template<size_t N1, size_t N2>
        void extra(char const (&tag)[N1],
                   char const (&text)[N2],
                   data_type const& data) {
            this->template print<severity::extra>(
                std::string_view {tag, N1 - 1},
                std::string_view {text, N2 - 1},
                data);
        }


        template<size_t N1, size_t N2>
        void trace(char const (&tag)[N1], char const (&text)[N2]) {
            this->template print<severity::trace>(
                std::string_view {tag, N1 - 1},
                std::string_view {text, N2 - 1});
        }


        template<size_t N1, size_t N2>
        void trace(char const (&tag)[N1],
                   char const (&text)[N2],
                   data_type const& data) {
            this->template print<severity::trace>(
                std::string_view {tag, N1 - 1},
                std::string_view {text, N2 - 1},
                data);
        }


#ifdef NDEBUG


        template<size_t N1, size_t N2>
        void debug(char const (&)[N1], char const (&)[N2]) {}


        template<size_t N1, size_t N2>
        void debug(char const (&)[N1], char const (&)[N2], data_type const&) {}


#else


        template<size_t N1, size_t N2>
        void debug(char const (&tag)[N1], char const (&text)[N2]) {
            this->template print<severity::debug>(
                std::string_view {tag, N1 - 1},
                std::string_view {text, N2 - 1});
        }


        template<size_t N1, size_t N2>
        void debug(char const (&tag)[N1],
                   char const (&text)[N2],
                   data_type const& data) {
            this->template print<severity::debug>(
                std::string_view {tag, N1 - 1},
                std::string_view {text, N2 - 1},
                data);
        }


#endif


    protected:

        template<chronicle::severity S>
        void print(std::string_view const& tag, std::string_view const& text) {
            if(severity_ < S)
                return;
            message_type* m = claim<S>(tag, text);
            if(!m)
                return;
            publish(*m);
        }


        template<chronicle::severity S>
        void print(std::string_view const& tag,
                   std::string_view const& text,
                   data_type const& data) {
            if(severity_ < S)
                return;
            message_type* m = claim<S>(tag, text);
            if(!m)
                return;
            m->data = data;
            m->has_data = true;
            publish(*m);
        }


        void publish(message_type const& m) {
            activity_.publish(m.sequence);
        }


        template<chronicle::severity S>
        message_type* claim(std::string_view const& source,
                            std::string_view const& text) {
            auto const sequence = activity_.claim();
            if(!sequence)
                return nullptr;
            message_type& m = activity_[sequence];
            m.sequence = sequence;
            m.severity = S;
#if defined(_WIN32)
            m.thread_id = unsigned(GetCurrentThreadId());
#elif defined(__linux__)
            m.thread_id = unsigned(gettid());
#endif
            m.source = source;
            m.text = text;
            m.has_data = false;
            return &m;
        }

    };   // data_log


    template<typename D>
    using unique_data_log = data_log<traits_unique_default<D>>;
    template<typename D>
    using shared_data_log = data_log<traits_shared_default<D>>;


}   // namespace chronicle
