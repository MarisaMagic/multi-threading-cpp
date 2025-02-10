#include <iostream>
#include <thread>
#include <mutex>

std::mutex mtx;
int count;

void foo() {
    std::unique_lock<std::mutex> lck(mtx);
    // 假设我们需要将锁的所有权转移给另一个对象
    std::mutex *m = lck.release(); // 释放锁所有权
    // 此时 lck 不再持有锁，转移到了 std::mutex 互斥量 m 上
    count ++ ;
    std::cout << "thread id = " << std::this_thread::get_id();
    std::cout << ", count = " << count << "\n";
    m->unlock(); // 需要手动释放锁
}

int main() {
    std::thread t1(foo);
    std::thread t2(foo);

    t1.join();
    t2.join();

    return 0;
}