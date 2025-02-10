#include <iostream>
#include <mutex>
#include <thread>
#include <chrono>

// 可以使用 unique_lock 暂时解锁，使得其他线程有机会获得锁
// 可以减少死锁的发生，但是无法解决死锁。

std::mutex mtx1, mtx2;

void foo1() {
    std::unique_lock<std::mutex> lck1(mtx1);
    std::cout << "thread 1 acquired mtx1" << std::endl;

    // 暂时解锁 mtx1
    lck1.unlock();
    std::cout << "thread 1 unlocked mtx1" << std::endl;

    // 暂停一会使得其他线程有机会获得 mtx1 
    std::this_thread::sleep_for(std::chrono::milliseconds(300));

    // 尝试获取 mtx2
    std::unique_lock<std::mutex> lck2(mtx2);
    std::cout << "thread 1 acquired mtx2" << std::endl;

    // 重新获取 mtx1 的锁
    lck1.lock();
    std::cout << "thread 1 reacquired mtx1" << std::endl;

    // 模拟进行一些需要同时持有两个互斥量的操作
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
}

void foo2() {
    std::unique_lock<std::mutex> lck2(mtx2);
    std::cout << "thread 2 acquired mtx2" << std::endl;

    // 暂时解锁 mtx2
    lck2.unlock();
    std::cout << "thread 2 unlocked mtx2" << std::endl;

    // 暂停一会使得其他线程有机会获得 mtx2 
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    // 尝试获取 mtx1
    std::unique_lock<std::mutex> lck1(mtx1);
    std::cout << "thread 2 acquired mtx1" << std::endl;

    // 重新获取 mtx2 的锁
    lck2.lock();
    std::cout << "thread 2 reacquired mtx2" << std::endl;

    // 模拟进行一些需要同时持有两个互斥量的操作
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
}

int main() {
    std::thread thread1(foo1);
    std::thread thread2(foo2);

    thread1.join();
    thread2.join();

    std::cout << "program ends." << "\n";

    return 0;
}