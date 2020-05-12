
#include <iostream>

extern "C" void before_add_func() {
    std::cout << "Call before add func" << std::endl;
}
