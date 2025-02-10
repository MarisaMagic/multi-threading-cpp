#include <atomic>
#include <thread>
#include <iostream>
#include <chrono>

std::atomic<int> atm1(0), atm2(0);
int res1 = 0, res2 = 0;

void foo1() {
    res1 = atm2.load(std::memory_order_acquire);
    atm1.store(1, std::memory_order_release);
}

void foo2() {
    res2 = atm1.load(std::memory_order_acquire);
    atm2.store(1, std::memory_order_release);
}

int main(){
    std::thread t1(foo1);
    std::thread t2(foo2);

    t1.join();
    t2.join();

    std::cout << "r1: " << res1 << ", r2: " << res2 << "\n";
    // (0, 0), (0, 1), (1, 0)
    // acq acq re re
    // acq re acq re

    return 0;
}
