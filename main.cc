
#include <iostream>
#include "task-joint.hpp"
#include "task-chain.hpp"

int main() {
    try {
        auto frame_ptr = __builtin_coro_noop();
        std::cout << frame_ptr << std::endl;
        auto promise = __builtin_coro_promise(nullptr, 0, true);
        std::cout << promise << std::endl;
        std::cout << __builtin_coro_done(frame_ptr) << std::endl;
        __builtin_coro_resume(frame_ptr);
        std::cout << __builtin_coro_done(frame_ptr) << std::endl;
        __builtin_coro_destroy(frame_ptr);
        std::cout << __builtin_coro_done(frame_ptr) << std::endl;
        /////////////////////////////////////////////////////////////////////////////
        std::cout << "-- joint test --" << std::endl;
        joint_test();
        /////////////////////////////////////////////////////////////////////////////
        std::cout << "-- chain test --" << std::endl;
        chain_test();
    }
    catch (std::exception& ex) {
        std::cerr << ex.what() << std::endl;
    }
    return 0;
}
