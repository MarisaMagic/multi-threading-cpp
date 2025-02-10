#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <chrono>

std::timed_mutex mtx;
int count;

void foo(std::chrono::steady_clock::time_point zero){
    std::chrono::milliseconds timeout(200);
    auto start = std::chrono::steady_clock::now();
    // 线程启动时间戳
    std::chrono::duration<double, std::milli> dura_zero = start - zero;
    std::cout << dura_zero.count() << "ms.\n";
    // 最长等待 200 ms 
    if(mtx.try_lock_for(timeout)){
        // 在等待的 200ms 期限内成功获得锁，并获得锁的等待时间（可能略超 200ms）
        auto end = std::chrono::steady_clock::now();
        std::chrono::duration<double, std::milli> dura = end - start;
        std::cout << "thread " << std::this_thread::get_id() << ": Successfully get the lock. ";
        std::cout << "waiting duration = " << dura.count() << "ms." << "\n";
        // 模拟一些互斥操作
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        count ++ ;
        mtx.unlock();
    }else{
        // std::cout << "thread id = " << std::this_thread::get_id();
        // std::cout << ": Unsuccessfully get the lock." << "\n";
    }
}

int main(){
    std::vector<std::thread> threads(20);
    for(size_t i = 0; i < threads.size(); i ++ ){
        std::chrono::steady_clock::time_point zero = std::chrono::steady_clock::now();
        threads[i] = std::thread(foo, zero);
    }
    for(auto& t : threads) t.join();

    std::cout << "final count = " << count << "\n";

    return 0;
}