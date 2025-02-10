#include <iostream>
#include <semaphore>
#include <thread>
#include <vector>

std::counting_semaphore<2> sem(2); // 最多允许2个线程同时访问

void foo(int id) {
    sem.acquire(); // 获取资源权限
    std::cout << "Thread " << id << " is accessing the resource\n";
    std::this_thread::sleep_for(std::chrono::seconds(1)); // 模拟资源访问
    std::cout << "Thread " << id << " is releasing the resource\n";
    sem.release(); // 释放资源权限
}

int main() {
    std::vector<std::thread> threads(5);
    for(int i = 0; i < 5; i ++ ) threads[i] = std::thread(foo, i);
    for(auto& t : threads) t.join();
    
    return 0;
}