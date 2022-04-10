
#include <iostream>
#include "task-joint.hpp"
#include "task-chain.hpp"

int main() {
    try {
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
