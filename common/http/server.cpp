//
// Created by zhaohuiqiang on 2022/1/17.
//
#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"
#pragma ide diagnostic ignored "modernize-avoid-bind"

#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread/thread.hpp>
#include <vector>

#include "server.hpp"

namespace http::server3 {

    server::server(const std::string &address, const std::string &port,
                   const std::string &doc_root, std::size_t thread_pool_size)
            : thread_pool_size_(thread_pool_size),
              signals_(io_context_),
              acceptor_(io_context_),
              new_connection_(),
              request_handler_(doc_root) {
        // Register to handle the signals that indicate when the server should exit.
        // It is safe to register for the same signal multiple times in a program,
        // provided all registration for the specified signal is made through Asio.
        signals_.add(SIGINT);
        signals_.add(SIGTERM);
#if defined(SIGQUIT)
        signals_.add(SIGQUIT);
#endif // defined(SIGQUIT)
        signals_.async_wait(boost::bind(&server::handle_stop, this));

        // Open the acceptor with the option to reuse the address (i.e. SO_REUSEADDR).
        boost::asio::ip::tcp::resolver resolver(io_context_);
        boost::asio::ip::tcp::endpoint endpoint =
                *resolver.resolve(address, port).begin();
        acceptor_.open(endpoint.protocol());
        acceptor_.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
        acceptor_.bind(endpoint);
        acceptor_.listen();

        start_accept();
    }

    void server::run() {
        // Create a pool of threads to run all the io_contexts.
        std::vector<boost::shared_ptr<boost::thread> > threads;
        for (std::size_t i = 0; i < thread_pool_size_; ++i) {
            boost::shared_ptr<boost::thread> thread(new boost::thread(
                    boost::bind(&boost::asio::io_context::run, &io_context_)));
            threads.push_back(thread);
        }

        // Wait for all threads in the pool to exit.
        for (auto &thread: threads)
            thread->join();
    }

    void server::start_accept() {
        new_connection_.reset(new connection(io_context_, request_handler_));
        acceptor_.async_accept(new_connection_->socket(),
                               boost::bind(&server::handle_accept, this,
                                           boost::asio::placeholders::error));
    }

    void server::handle_accept(const boost::system::error_code &e) {
        if (!e) {
            new_connection_->start();
        }

        start_accept();
    }

    void server::handle_stop() {
        io_context_.stop();
    }

}