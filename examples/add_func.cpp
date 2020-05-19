
#include <iostream>

using namespace std;

extern "C" int add_func(int a, int b) {

    cout << "Call new extra add func" << endl;
    int result = (a + b) * 2;
    cout << "Result: " << result << endl;
    return result;
}


