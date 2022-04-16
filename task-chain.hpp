#ifndef INCLUDE_TASK_CHAIN_HPP
#define INCLUDE_TASK_CHAIN_HPP

#include <utility>
#include "coroutine-compatible.hpp"

inline namespace task_chain
{
    struct task {
        struct promise_type {
            std::coroutine_handle<> continuation;
            void unhandled_exception() { throw; }
            auto get_return_object() { return task{*this}; }
            auto initial_suspend() { return std::suspend_always{}; }
            auto final_suspend() noexcept { return std::suspend_always{}; }
            auto yield_value(bool cont) {
                struct awaiter {
                    std::coroutine_handle<> continuation;
                    bool await_ready() { return false; }
                    auto await_suspend(std::coroutine_handle<>) { return this->continuation; }
                    void await_resume() {}
                };
                return awaiter{ cont ? this->continuation : std::noop_coroutine() };
            }
            void return_void() { }
        };
        ~task() {
            if (this->handle) this->handle.destroy();
        }
        task(task const&) = delete;
        task(task&& rhs)
            : handle{std::exchange(rhs.handle, nullptr)}
        {
        }
        void set_next(task& t) {
            this->handle.promise().continuation = t.handle;
        }
        void start() {
            if (!this->handle.done()) this->handle.resume();
        }
    private:
        explicit task(promise_type& p)
            : handle{std::coroutine_handle<promise_type>::from_promise(p)}
        {
        }
        std::coroutine_handle<promise_type> handle;
    };
    inline void chain_test() {
        auto chain = [](int id) -> task {
            int n = id * 16;
            for (;;) {
                std::cout << "coro#" << id << ' ' << n << std::endl;
                co_yield (0 < n);
                n /= 2;
            }
        };
        auto c1 = chain(1);
        auto c2 = chain(2);
        auto c3 = chain(3);
        c1.set_next(c2);
        c2.set_next(c3);
        c3.set_next(c1);
        c1.start();
    }
} // ::task_chain

#endif/*INCLUDE_TASK_CHAIN_HPP*/
