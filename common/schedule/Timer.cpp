//
// Created by 赵恢强 on 2022/6/15.
//
#include <thread>

#include "Timer.h"

namespace schedule {
    [[maybe_unused]] Timer::Timer(boost::asio::io_service &ioService,
                                  const function<void()> &task,
                                  boost::posix_time::time_duration &duration_)
            : ioService_(ioService), timer_(ioService), task_(task),
              duration_(duration_) {}

    [[maybe_unused]] Timer::Timer(boost::asio::io_service &ioService,
                                  const function<void()> &task, uint64_t &milliSeconds)
            : ioService_(ioService), timer_(ioService), task_(task),
              duration_(boost::posix_time::milliseconds(milliSeconds)) {}

    [[maybe_unused]] Timer::Timer(boost::asio::io_service &ioService,
                                  const function<void()> &task, uint32_t &seconds)
            : ioService_(ioService), timer_(ioService), task_(task),
              duration_(boost::posix_time::seconds(seconds)) {}

    [[maybe_unused]] Timer::Timer(boost::asio::io_service &ioService,
                                  const function<void()> &task, uint16_t &minutes)
            : ioService_(ioService), timer_(ioService), task_(task),
              duration_(boost::posix_time::minutes(minutes)) {}

    [[maybe_unused]] Timer::Timer(boost::asio::io_service &ioService,
                                  const function<void()> &task, uint8_t &hours)
            : ioService_(ioService), timer_(ioService), task_(task),
              duration_(boost::posix_time::hours(hours)) {}

    void Timer::run() {
        thread thd([this] {
            start(boost::bind(&Timer::onRun, this, _1));
            ioService_.run();
        });
        thd.detach();
    }

    [[maybe_unused]] void Timer::stop() {
        try {
            timer_.cancel();
        } catch (...) {
        }
    }

    void Timer::start(function<void(const boost::system::error_code &)> cb) {
        timer_.expires_from_now(duration_);
        timer_.async_wait(cb);
    }

    void Timer::onRun(const boost::system::error_code &ec) {
        if (ec != boost::asio::error::operation_aborted) {
            start(boost::bind(&Timer::onRun, this, _1));
            ioService_.post(task_);
        }
    }

} // namespace schedule
