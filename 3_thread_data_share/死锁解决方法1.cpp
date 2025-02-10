#include <iostream>
#include <mutex>
#include <thread>

std::mutex mtx1, mtx2;

// 方法1：两个线程对互斥量加锁的顺序一致，可以避免死锁

void foo1(){
    std::lock_guard<std::mutex> lck1(mtx1);
    std::cout << "thread 1 get mtx1." << "\n";
    std::lock_guard<std::mutex> lck2(mtx2);
    std::cout << "thread 1 get mtx2." << "\n"; 
        
    // 模拟进行一些需要同时持有两个互斥量的操作
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
}

void foo2(){
    std::lock_guard<std::mutex> lck1(mtx1);
    std::cout << "thread 2 get mtx1." << "\n";
    std::lock_guard<std::mutex> lck2(mtx2);
    std::cout << "thread 2 get mtx2." << "\n"; 
        
    // 模拟进行一些需要同时持有两个互斥量的操作
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
}

int main(){
    std::thread t1(foo1);
    std::thread t2(foo2);
    t1.join();
    t2.join();

    std::cout << "program ends." << "\n";

    return 0;
}