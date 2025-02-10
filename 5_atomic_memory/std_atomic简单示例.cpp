#include <iostream>
#include <thread>
#include <atomic>

std::atomic<int> atm(1);

void foo() {
    int old_val = atm.exchange(2);
    std::cout << "Old value: " << old_val << "\n"; // 1
    std::cout << "New value: " << atm.load() << "\n"; // 2
}

int main() {
    std::thread t(foo);
    t.join();

    return 0;
}