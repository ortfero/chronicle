// This file is part of chronicle library
// Copyright 2020-2022 Andrei Ilin <ortfero@gmail.com>
// SPDX-License-Identifier: MIT

#pragma once


#include <chrono>
#include <cstdio>
#include <filesystem>
#include <memory>

#include <date/date.h>
#include <ufmt/text.hpp>

#include <chronicle/sink.hpp>


namespace chronicle::sinks {


    class daily_rotated_file: public sink {
        using handle_type = FILE*;
        static constexpr auto invalid_handle = nullptr;

        handle_type handle_ {invalid_handle};
        std::filesystem::path directory_;
        std::filesystem::path base_name_;
        std::filesystem::path extension_;
        std::filesystem::path file_path_;
        uint64_t log_day_ {0};
        unsigned part_ {1};
        std::size_t limit_ {0};
        std::size_t written_ {0};

    public:
        static expected_sink_ptr open(std::filesystem::path const& path,
                                      std::size_t limit = 0) {
            std::error_code ec;
            daily_rotated_file drf{path, limit, ec};
            if(!drf.ready())
                return tl::make_unexpected(ec);
            return {sink_ptr{new daily_rotated_file{std::move(drf)}}};
        }


        daily_rotated_file() noexcept = default;
        ~daily_rotated_file() override { close(); }
        daily_rotated_file(daily_rotated_file const&) noexcept = delete;
        daily_rotated_file&
            operator=(daily_rotated_file const&) noexcept = delete;

        std::filesystem::path const& file_path() const noexcept {
            return file_path_;
        }


        daily_rotated_file(daily_rotated_file&& other) noexcept
            : handle_ {other.handle_},
              directory_ {std::move(other.directory_)},
              base_name_ {std::move(other.base_name_)},
              extension_ {std::move(other.extension_)},
              file_path_ {std::move(other.file_path_)},
              log_day_ {other.log_day_},
              part_ {other.part_},
              limit_ {other.limit_},
              written_ {other.written_} {
            other.handle_ = invalid_handle;
        }


        daily_rotated_file& operator=(daily_rotated_file&& other) noexcept {
            if(handle_ != invalid_handle)
                std::fclose(handle_);
            handle_ = other.handle_;
            other.handle_ = invalid_handle;
            directory_ = std::move(other.directory_);
            base_name_ = std::move(other.base_name_);
            extension_ = std::move(other.extension_);
            file_path_ = std::move(other.file_path_);
            log_day_ = other.log_day_;
            part_ = other.part_;
            limit_ = other.limit_;
            written_ = other.written_;
            return *this;
        }


        bool ready() const noexcept override {
            return handle_ != invalid_handle;
        }


        void write(time_point const& tp,
                   char const* data,
                   size_t size) noexcept override {
            using namespace std::chrono;
            std::error_code ec;
            auto const now_day = uint64_t(
                duration_cast<hours>(tp.time_since_epoch()).count() / 24);
            if(now_day != log_day_) {
                log_day_ = now_day;
                part_ = 1;
                rotate_file(tp, ec);
            } else if(limit_ != 0 && written_ + size > limit_) {
                ++part_;
                rotate_file(tp, ec);
            }

            std::fwrite(data, sizeof(char), size, handle_);

            written_ += size;
        }


        void flush() noexcept override {
            std::fflush(handle_);
        }


        void close() noexcept override {
            if(handle_ == invalid_handle)
                return;
            std::fclose(handle_);
            handle_ = invalid_handle;
        }


        void prologue(const char* data, size_t size) noexcept override {
            std::fwrite(data, sizeof(char), size, handle_);
        }


        void epilogue(const char* data, size_t size) noexcept override {
            std::fwrite(data, sizeof(char), size, handle_);
        }


    private:
    
        daily_rotated_file(std::filesystem::path const& path,
                           std::size_t limit,
                           std::error_code& ec) noexcept {
            namespace fs = std::filesystem;
            directory_ = path.parent_path();
            if(!directory_.empty() && !fs::exists(directory_))
                fs::create_directories(directory_, ec);
            if(!!ec)
                return;
            base_name_ = path.stem();
            extension_ = path.extension();
            using namespace std::chrono;
            auto const now = system_clock::now();
            log_day_ =
                duration_cast<hours>(now.time_since_epoch()).count() / 24;
            limit_ = limit;
            rotate_file(now, ec);
        }


        void rotate_file(std::chrono::system_clock::time_point tp,
                         std::error_code& ec) {
            if(handle_ != invalid_handle)
                fclose(handle_);
            auto const dp = std::chrono::floor<date::days>(tp);
            date::year_month_day const ymd = dp;
            ufmt::text suffix;
            suffix << '-' << int(ymd.year()) << '_';
            if(unsigned(ymd.month()) < 10)
                suffix << '0';
            suffix << unsigned(ymd.month()) << '_';
            if(unsigned(ymd.day()) < 10)
                suffix << '0';
            suffix << unsigned(ymd.day());
            if(part_ != 1) {
                suffix << '-';
                if(part_ < 10)
                    suffix << '0';
                suffix << part_;
            }
            file_path_ = directory_;
            file_path_ /= base_name_;
            file_path_ += suffix.string();
            file_path_ += extension_;
            handle_ = std::fopen(file_path_.string().data(), "ab+");
            if(handle_ == nullptr)
                ec = {errno, std::system_category()};
            written_ = 0;
        }


    };   // daily_rotated_file

}   // namespace chronicle::sinks
