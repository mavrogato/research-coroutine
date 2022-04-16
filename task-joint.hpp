#ifndef INCLUDE_TASK_JOINT_HPP
#define INCLUDE_TASK_JOINT_HPP

#include <utility>
#include "coroutine-compatible.hpp"

inline namespace task_joint
{
    template <class T>
    struct task {
        struct promise_type {
            std::coroutine_handle<> continuation;
            T result;
            void unhandled_exception() { throw; }
            auto get_return_object() { return task{*this}; }
            auto initial_suspend() { return std::suspend_always{}; }
            auto final_suspend() noexcept {
                struct awaiter {
                    bool await_ready() noexcept { return false; }
                    void await_resume() noexcept { }
                    auto await_suspend(std::coroutine_handle<promise_type> handle) noexcept {
                        /* 
                           awaiter::await_suspend is called when the execution of the
                           current coroutine (referred to by 'handle') is about to finish.
                           If the current coroutine was resumed by another coroutine via
                           co_await get_task(), a handle to that coroutine has been stored
                           as handle.promise().continuation. In that case, return the handle
                           to resume the previous coroutine.
                           Otherwise, return noop_coroutine(), whose resumption does nothing.
                        */
                        std::coroutine_handle<> continuation = handle.promise().continuation;
                        if (!continuation) {
                            continuation = std::noop_coroutine();
                        }
                        return continuation;
                    }
                };
                return awaiter{};
            }
            void return_value(T value) { this->result = std::move(value); }
        };
        ~task() {
            if (this->handle) this->handle.destroy();
        }
        task(task const&) = delete;
        task(task&& rhs)
            : handle{std::exchange(rhs.handle, nullptr)}
        {
        }
        auto operator co_await() {
            struct awaiter {
                bool await_ready() { return false; }
                T await_resume() { return std::move(this->handle.promise().result); }
                auto await_suspend(std::coroutine_handle<> handle) {
                    this->handle.promise().continuation = handle;
                    return this->handle;
                }
                std::coroutine_handle<promise_type> handle;
            };
            return awaiter{this->handle};
        }
        T operator()() {
            this->handle.resume();
            return std::move(this->handle.promise().result);
        }
    private:
        explicit task(promise_type& p)
            : handle{std::coroutine_handle<promise_type>::from_promise(p)}
        {
        }
        std::coroutine_handle<promise_type> handle;
    };
    inline void joint_test() {
        auto joint = []() -> task<int> {
            auto get_random = []() -> task_joint::task<int> {
                std::cout << "in get_random()" << std::endl;
                co_return 4;
            };
            auto v = get_random();
            auto u = get_random();
            std::cout << "in joint()" << std::endl;
            int x = co_await v;
            int y = co_await u;
            std::cout << " x=" << x << std::endl;
            std::cout << " y=" << y << std::endl;
            co_return x + y;
        };
        auto t = joint();
        int result = t();
        std::cout << "result=" << result << std::endl;
    }
} // ::task_joint

#endif/*INCLUDE_TASK_JOINT_HPP*/
