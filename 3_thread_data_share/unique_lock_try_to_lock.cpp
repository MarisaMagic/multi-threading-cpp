#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <chrono>

std::mutex mtx;
int count;

void foo(){
    // 定义 unique_lock，加上 std::try_to_lock
    std::unique_lock<std::mutex> lck(mtx, std::try_to_lock);
    if(lck.owns_lock()){ 
        // 成功获得锁
        count ++ ;
        std::cout << "thread id = " << std::this_thread::get_id();
        std::cout << ", Successfully get the lock." << "\n";
    }else{ 
        // 未能获得锁，不等待，线程直接返回
        std::cout << "thread id = " << std::this_thread::get_id();
        std::cout << ", Unsuccessfully get the lock." << "\n";
    }
}

int main(){
    std::vector<std::thread> v(30);
    for(auto& t : v) t = std::thread(foo);
    for(auto& t : v) t.join();

    std::cout << "final count = " << count << "\n";

    return 0;
}