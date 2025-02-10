#include <iostream>
#include <thread>
#include <mutex>

/*
    release 函数用于将与 std::unique_lock 关联的互斥量的所有权转移出去。
    调用 release 后，std::unique_lock 对象将不再持有锁，也不会在析构时自动释放锁。
    这通常用于将锁传递给其他对象或函数，或者在某些情况下手动管理锁的生命周期。
*/

std::mutex mtx;

void release_example() {
    std::unique_lock<std::mutex> lck(mtx); // 默认立即获取锁

    // 假设我们需要将锁的所有权转移给另一个对象
    std::mutex *m = lck.release(); // 释放锁所有权

    // 此时，lck 不再持有锁
    if (m->try_lock()) {
        std::cout << "Lock acquired again after release\n";
        m->unlock(); // 手动释放锁
    } else {
        std::cout << "Failed to acquire lock after release\n";
    }
}

int main() {
    std::thread t1(release_example);
    std::thread t2(release_example);

    t1.join();
    t2.join();

    return 0;
}