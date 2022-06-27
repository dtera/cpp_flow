//
// Created by 赵恢强 on 2022/6/15.
//
#pragma once

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/function.hpp>

using namespace std;

namespace schedule {
    class [[maybe_unused]] Timer {
    public:
        [[maybe_unused]] Timer(boost::asio::io_service &ioService,
                               const function<void()> &task,
                               boost::posix_time::time_duration &duration);

        [[maybe_unused]] Timer(boost::asio::io_service &ioService,
                               const function<void()> &task, uint64_t &milliSeconds);

        [[maybe_unused]] Timer(boost::asio::io_service &ioService,
                               const function<void()> &task, uint32_t &seconds);

        [[maybe_unused]] Timer(boost::asio::io_service &ioService,
                               const function<void()> &task, uint16_t &minutes);

        [[maybe_unused]] Timer(boost::asio::io_service &ioService,
                               const function<void()> &task, uint8_t &hours);

        [[maybe_unused]] void run();

        [[maybe_unused]] void stop();

    private:
        [[maybe_unused]] boost::asio::io_service &ioService_;
        [[maybe_unused]] boost::asio::deadline_timer timer_;
        [[maybe_unused]] boost::posix_time::time_duration duration_;
        [[maybe_unused]] function<void()> task_{};

        [[maybe_unused]] void start(
                function<void(const boost::system::error_code &)> cb);

        [[maybe_unused]] void onRun(const boost::system::error_code &ec);
    };
} // namespace schedule
