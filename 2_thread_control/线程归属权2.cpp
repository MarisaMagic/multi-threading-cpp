#include <iostream>
#include <thread>

void foo() {
    std::cout << "Kirisame Marisa" << std::endl;
}

int main() {
    std::thread t1(foo);
    std::thread t2 = std::move(t1);  // t1的线程归属权从转移到t2
    // t1 变成空线程，不关联任何线程函数；t2 关联线程函数 foo
    // t1 是 unjoinable 的
    t2.join();
    
    return 0;
}