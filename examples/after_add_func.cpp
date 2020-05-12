
#include <iostream>

extern "C" void after_add_func() {
    std::cout << "Call after add func" << std::endl;
}
