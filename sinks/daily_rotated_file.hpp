// This file is part of chronicle library
// Copyright 2020-2024 Andrei Ilin <ortfero@gmail.com>
// SPDX-License-Identifier: MIT

#pragma once


#include <chrono>
#include <cstdio>
#include <filesystem>
#include <memory>
#include <sys/stat.h>
#include <sys/types.h>

#include <ufmt/print.hpp>
#include <ufmt/text.hpp>

#include <chronicle/sink.hpp>


namespace chronicle::sinks {


    class daily_rotated_file: public sink {
        FILE* handle_ {nullptr};
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
            daily_rotated_file drf {path, limit, ec};
            if(!drf.ready())
                return etceteras::make_unexpected(ec);
            return {sink_ptr {new daily_rotated_file {std::move(drf)}}};
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
            other.handle_ = nullptr;
        }


        daily_rotated_file& operator=(daily_rotated_file&& other) noexcept {
            if(handle_ != nullptr)
                std::fclose(handle_);
            handle_ = other.handle_;
            other.handle_ = nullptr;
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


        bool ready() const noexcept override { return handle_ != nullptr; }


        void write(time_point const& tp,
                   char const* data,
                   size_t size) noexcept override {
            if(!handle_)
                return;
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
            if(!handle_)
                return;
            std::fflush(handle_);
        }


        void close() noexcept override {
            if(!handle_)
                return;
            std::fclose(handle_);
            handle_ = nullptr;
        }


        void prologue(const char* data, size_t size) noexcept override {
            if(!handle_)
                return;
            std::fwrite(data, sizeof(char), size, handle_);
        }


        void epilogue(const char* data, size_t size) noexcept override {
            if(!handle_)
                return;
            std::fwrite(data, sizeof(char), size, handle_);
        }


    private:
        daily_rotated_file(std::filesystem::path const& path,
                           std::size_t limit,
                           std::error_code& ec) noexcept {
            namespace fs = std::filesystem;
            directory_ = path.parent_path();
            if(!directory_.empty() && !fs::exists(directory_)) {
                fs::create_directories(directory_, ec);
            }
            if(!!ec)
                return;
            base_name_ = path.stem();
            extension_ = path.extension();
            namespace chr = std::chrono;
            auto const now = chr::system_clock::now();
            log_day_ =
                chr::duration_cast<chr::hours>(now.time_since_epoch()).count()
                / 24;
            limit_ = limit;
            rotate_file(now, ec);
        }


        void rotate_file(std::chrono::system_clock::time_point tp,
                         std::error_code& ec) {
            if(handle_)
                std::fclose(handle_);
            namespace chr = std::chrono;
            auto const dp = chr::floor<chr::days>(tp);
            auto const ymd = chr::year_month_day {dp};
            auto suffix = ufmt::text {};
            suffix << '-' << int(ymd.year()) << '_';
            if(unsigned(ymd.month()) < 10)
                suffix << '0';
            suffix << unsigned(ymd.month()) << '_';
            if(unsigned(ymd.day()) < 10)
                suffix << '0';
            suffix << unsigned(ymd.day());
            auto full_path_without_part = directory_;
            full_path_without_part /= base_name_;
            full_path_without_part += suffix.string();

            while(!fits_to_open(file_path_,
                                full_path_without_part,
                                part_,
                                limit_,
                                written_,
                                extension_))
                ++part_;

            handle_ = std::fopen(file_path_.string().data(), "a+b");
            if(!handle_) {
                ec = {errno, std::system_category()};
                return;
            }
        }


        static bool
            fits_to_open(std::filesystem::path& file_path,
                         std::filesystem::path const& full_path_without_part,
                         unsigned part,
                         std::size_t limit,
                         std::size_t& written,
                         std::filesystem::path const& extension) {
            namespace fs = std::filesystem;
            file_path = full_path_without_part;
            if(part != 1) {
                auto part_text = ufmt::text {};
                part_text << '-';
                if(part < 10)
                    part_text << '0';
                part_text << part;
                file_path += part_text.string();
            }
            file_path += extension;
            auto ec = std::error_code {};
            auto const file_size = fs::file_size(file_path, ec);
            if(ec) {
                written = 0;
                return true;
            }
            if(file_size + 1024 >= limit)
                return false;
            written = file_size;
            return true;
        }

    };   // daily_rotated_file

}   // namespace chronicle::sinks
