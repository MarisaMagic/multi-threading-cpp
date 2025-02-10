#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <chrono>

std::timed_mutex t_mtx;
int count;

void foo(int id){
    std::unique_lock<std::timed_mutex> lck(t_mtx, std::defer_lock);
    // 最长等待时间期限设置为 100 ms 
    std::chrono::milliseconds timeout(100);
    auto start = std::chrono::steady_clock::now();
    if(t_mtx.try_lock_for(timeout)){ // 100ms 期限内成功获得锁

        // 以下模拟一些互斥操作
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        count ++ ;       // 对共享数据变量进行访问和修改
    }
    auto end = std::chrono::steady_clock::now();
    std::chrono::duration<double, std::milli> dura = end - start;
    std::cout << "thread " << id << ": get the lock. ";
    std::cout << "duration = " << dura.count() << "ms." << "\n";
}

int main(){
    std::vector<std::thread> threads(10);
    for(size_t i = 0; i < threads.size(); i ++ ){
        threads[i] = std::thread(foo, i);
    }
    for(auto& t : threads) t.join();

    std::cout << "final count = " << count << "\n";

    return 0;
}