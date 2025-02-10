#include <iostream>
#include <mutex>
#include <thread>

std::mutex mtx1, mtx2;

// 方法2：两个线程对互斥量加锁时，同时对需要的所有互斥量一次性加锁 std::lock
// 最后每个锁都要释放，解锁的顺序可以是任意的

void foo1(){
    std::lock(mtx1, mtx2);
    std::cout << "thread 1 get mtx1." << "\n";
    std::cout << "thread 1 get mtx2." << "\n"; 

    // 模拟进行一些需要同时持有两个互斥量的操作
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    mtx1.unlock();
    mtx2.unlock();
}

void foo2(){
    std::lock(mtx1, mtx2);
    std::cout << "thread 2 get mtx1." << "\n";
    std::cout << "thread 2 get mtx2." << "\n"; 

    // 模拟进行一些需要同时持有两个互斥量的操作
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    
    mtx2.unlock();
    mtx1.unlock();
}

int main(){
    std::thread t1(foo1);
    std::thread t2(foo2);
    t1.join();
    t2.join();

    std::cout << "program ends." << "\n";

    return 0;
}