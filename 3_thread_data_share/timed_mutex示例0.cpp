#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <chrono>

std::timed_mutex t_mtx;
int count;

void foo(int id){
    if(t_mtx.try_lock()){ 
        std::cout << "thread " << id << ": Successfully get the lock." << "\n";

        // 以下模拟一些互斥操作
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        count ++ ;     // 对共享数据变量进行访问和修改
        t_mtx.unlock();  // 解锁
    }else{
        std::cout << "thread " << id << ": Unsuccessfully get the lock." << "\n";
    }
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