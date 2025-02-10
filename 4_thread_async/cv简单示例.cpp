#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>

std::mutex mtx; // 互斥量
std::condition_variable cv; // 条件变量
bool flag; // 判断的条件

// 等待条件线程函数
void wait_foo(){
    std::unique_lock<std::mutex> lck(mtx);
    std::cout << "wait_foo(): waiting..." << "\n";
    cv.wait(lck, []{ return flag; });
    std::cout << "wait_foo(): waiting ends, flag = " << flag << "\n";
}

// 通知唤醒线程函数
void notify_foo(){
    // 第一次尝试唤醒。但是不满足条件，唤醒失败
    std::this_thread::sleep_for(std::chrono::seconds(2)); 
    {
        std::lock_guard<std::mutex> lck(mtx);
        std::cout << "notify_foo(): notifying..." << "\n";
    }
    cv.notify_one(); // 作用域结束解锁后再唤醒更好

    // 第二次尝试唤醒。满足条件，成功唤醒。
    std::this_thread::sleep_for(std::chrono::seconds(2)); 
    {
        std::lock_guard<std::mutex> lck(mtx);
        flag = true;
        std::cout << "notify_foo(): notifying again..." << "\n";
    }
    cv.notify_one();
}

int main(){
    std::thread wait_t(wait_foo);
    std::thread notify_t(notify_foo);
    wait_t.join();
    notify_t.join();

    return 0;
}