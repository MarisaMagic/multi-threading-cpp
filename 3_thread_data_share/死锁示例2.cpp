#include <iostream>
#include <mutex>
#include <thread>

std::mutex mtx1, mtx2;

// 当线程 1 持有 mtx1，等待获取 mtx2。同时线程 2 持有 mtx2，等待获取 mtx1 时
// 此时形成了循环等待，导致了死锁

void foo1(){
    std::lock_guard<std::mutex> lck1(mtx1);
    std::cout << "thread 1 get mtx1." << "\n";
    // 当前线程持有 mtx1 的锁，等待获取 mtx2 的锁
    std::lock_guard<std::mutex> lck2(mtx2);
    std::cout << "thread 1 get mtx2." << "\n";

    // 模拟进行一些需要同时持有两个互斥量的操作
    std::this_thread::sleep_for(std::chrono::milliseconds(100)); 
}

void foo2(){
    std::lock_guard<std::mutex> lck1(mtx2);
    std::cout << "thread 2 get mtx2." << "\n";
    // 当前线程持有 mtx2 的锁，等待获取 mtx1 的锁
    std::lock_guard<std::mutex> lck2(mtx1);
    std::cout << "thread 2 get mtx1." << "\n"; 

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