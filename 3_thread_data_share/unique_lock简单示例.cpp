#include <iostream>
#include <thread>
#include <mutex>
#include <vector>

std::mutex mtx; // 互斥量
int count;

void foo(){
    // 默认情况下和 lock_guard 一样，自动加锁、解锁
    std::unique_lock<std::mutex> lck(mtx);
    count ++ ;
    std::cout << "thread id = " << std::this_thread::get_id();
    std::cout << ", count = " << count << "\n"; 
}

int main(){
    std::vector<std::thread> v(10);
    for(auto& t : v) t = std::thread(foo);
    for(auto& t : v) t.join();

    return 0;
}