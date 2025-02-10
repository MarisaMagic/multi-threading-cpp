#include <iostream>
#include <mutex>
#include <thread>

std::mutex mtx1, mtx2;

// 方法2：两个线程对互斥量加锁时，同时对需要的所有互斥量一次性加锁 std::lock
// 最后每个锁都要释放，解锁的顺序可以是任意的
// 可以用 lock_guard 转移锁的管理，加上 std::adopt_lock 参数
// lock_guard 只能使用 adopt_lock，其他 defer_lock 等不能使用

void foo1(){
    std::lock(mtx1, mtx2);
    std::lock_guard<std::mutex> lck1(mtx1, std::adopt_lock);
    std::lock_guard<std::mutex> lck2(mtx2, std::adopt_lock);
    std::cout << "thread 1 get mtx1." << "\n";
    std::cout << "thread 1 get mtx2." << "\n"; 
        
    // 模拟进行一些需要同时持有两个互斥量的操作
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
}

void foo2(){
    std::lock(mtx1, mtx2);
    std::lock_guard<std::mutex> lck1(mtx2, std::adopt_lock);
    std::lock_guard<std::mutex> lck2(mtx1, std::adopt_lock);
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