// This file is part of chronicle library
// Copyright 2020-2022 Andrei Ilin <ortfero@gmail.com>
// SPDX-License-Identifier: MIT

#pragma once


#include <cstdio>
#include <filesystem>
#include <memory>
#include <system_error>

#include <chronicle/sink.hpp>


namespace chronicle::sinks {


    class file: public sink {
        using handle_type = FILE*;
        static constexpr auto invalid_handle = nullptr;

        handle_type handle_ {invalid_handle};

    public:
    
        static expected_sink_ptr open(std::filesystem::path const& path) noexcept {
            std::error_code ec;
            file f{path, ec};
            if(!f.ready())
                return tl::make_unexpected(ec);
            return {sink_ptr{new file {std::move(f)}}};
        }


        file() noexcept = default;
        ~file() override { close(); }

        file(file const&) noexcept = delete;
        file& operator=(file const&) noexcept = delete;

        file(file&& other) noexcept: handle_ {other.handle_} {
            other.handle_ = invalid_handle;
        }


        file& operator=(file&& other) noexcept {
            if(handle_ != invalid_handle)
                std::fclose(handle_);
            handle_ = other.handle_;
            other.handle_ = invalid_handle;
            return *this;
        }


        bool ready() const noexcept override {
            return handle_ != invalid_handle;
        }


        void write(time_point const&,
                   char const* data,
                   size_t size) noexcept override {
            std::fwrite(data, sizeof(char), size, handle_);
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

        file(std::filesystem::path const& path,
             std::error_code& error) noexcept {
            auto const directory = path.parent_path();
            namespace fs = std::filesystem;
            if(!directory.empty() && !fs::exists(directory))
                fs::create_directories(directory, error);
            if(!!error)
                return;
            handle_ = std::fopen(path.string().data(), "ab+");
            if(handle_ == nullptr)
                error = {errno, std::system_category()};
        }

    };   // file


}   // namespace chronicle::sinks
