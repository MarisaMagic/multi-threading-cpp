#include <iostream>
#include <thread>
#include <mutex>
#include <vector>

std::mutex mtx;
int count;

void foo() {
    std::unique_lock<std::mutex> lck(mtx, std::defer_lock);
    if(lck.try_lock()){
        // 成功获得锁
        count ++ ;
        std::cout << "Successfully get the lock." << "\n";
        std::cout << "thread id = " << std::this_thread::get_id();
        std::cout << ", count = " << count << "\n";
    }else{
        // 无法获得锁，直接返回
        std::cout << "Unsuccessfully get the lock." << "\n";
    }
}

int main() {
    std::vector<std::thread> v(10);
    for(auto& t : v) t = std::thread(foo);
    for(auto& t : v) t.join();

    return 0;
}