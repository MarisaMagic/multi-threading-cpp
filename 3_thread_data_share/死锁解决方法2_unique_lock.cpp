#include <iostream>
#include <mutex>
#include <thread>

std::mutex mtx1, mtx2;

// 方法2：两个线程对互斥量加锁时，同时对需要的所有互斥量一次性加锁 std::lock
// 最后每个锁都要释放，解锁的顺序可以是任意的
// 可以配合 unique_lock 延迟加锁，加上 std::defer_lock 参数
// 先定义 unique_lock 对象并加上延迟加锁参数
// 然后一次性对所需互斥量同时进行加锁

void foo1(){
    std::unique_lock<std::mutex> lck1(mtx1, std::defer_lock);
    std::unique_lock<std::mutex> lck2(mtx2, std::defer_lock);
    std::lock(lck1, lck2);
    std::cout << "thread 1 get mtx1." << "\n";
    std::cout << "thread 1 get mtx2." << "\n"; 

    // 模拟进行一些需要同时持有两个互斥量的操作
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
}

void foo2(){
    std::unique_lock<std::mutex> lck1(mtx1, std::defer_lock);
    std::unique_lock<std::mutex> lck2(mtx2, std::defer_lock);
    std::lock(lck2, lck1);
    std::cout << "thread 2 get mtx1." << "\n";
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