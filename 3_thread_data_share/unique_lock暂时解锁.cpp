#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <chrono>

std::mutex mtx;
int count;

void foo(){
    std::unique_lock<std::mutex> lck(mtx);
    count ++ ;
    std::cout << "thread id = " << std::this_thread::get_id();
    std::cout << ", count = " << count << "\n";

    lck.unlock(); // 暂时解锁

    // 模拟做一些其他不需要锁保护的操作
    std::this_thread::sleep_for(std::chrono::milliseconds(200));

    lck.lock();   // 重新加锁
    count ++ ;
    std::cout << "thread id = " << std::this_thread::get_id();
    std::cout << ", count = " << count << "\n";
    // 作用域结束 return 时自动解锁
}

int main(){
    std::vector<std::thread> v(10);
    for(auto& t : v) t = std::thread(foo);
    for(auto& t : v) t.join();

    std::cout << "final value = " << count << "\n";

    return 0;
}