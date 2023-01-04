#include "display.h"
#include <chrono>
#include <future>
#include <iostream>
#include <string>
#include <thread>

using namespace std;
using std::chrono::microseconds;
using std::chrono::milliseconds;
using std::chrono::seconds;
using std::this_thread::sleep_for;

class NormalLoop {
public:
    bool activate = true;
    void normal_loop()
    {
        while (activate) {
            auto now = std::chrono::system_clock::now();
            std::time_t end_time = std::chrono::system_clock::to_time_t(now);
            display("%s", std::ctime(&end_time));
            sleep_for(seconds(1));
        }
        display("Finish!");
        return;
    }
};

int main()
{
    auto nloop = NormalLoop();
    std::future f1 = std::async(std::launch::async, [&nloop]() -> void {
        nloop.normal_loop();
    });
    sleep_for(seconds(5));
    nloop.activate = false;
    return 0;
}