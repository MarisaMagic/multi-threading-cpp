#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <atomic>

std::mutex mtx;
std::atomic<bool> flag;
int val;

void foo(){
    if(!flag){
        std::lock_guard<std::mutex> lck(mtx);
        if(!flag){
            std::cout << "value initializing ... " << "\n";
            val = 1;
            std::cout << "value initialized as: " << val << '\n';
            flag = true;
        }
    }
}

void foo2(){
    if(!flag.load(std::memory_order_relaxed)){
        std::lock_guard<std::mutex> lck(mtx);
        if(!flag.load(std::memory_order_acquire)){
            std::cout << "value initializing ... " << "\n";
            val = 1;
            std::cout << "value initialized as: " << val << '\n';
            flag.store(true, std::memory_order_release);
        }
    }
}

int main(){
    std::vector<std::thread> threads(30);
    for(auto& t : threads) t = std::thread(foo2);
    for(auto& t : threads) t.join();

    return 0;
}