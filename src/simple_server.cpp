#include <iostream>
#include <chrono>
#include <thread>

#include "hotpatch_server.h"

using namespace std;

int main() {

    cout << "Start main" << endl;

    auto hp = make_shared<HotpatchServer>();

    hp->init();


    for(int i=0; i<10; i++) {
        cout << "Sleep for 1 second" << endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }


    hp->close();

    cout << "End of main" << endl;

}

