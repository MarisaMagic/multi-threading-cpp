#include <iostream>
#include <thread>
#include <shared_mutex>
#include <chrono>
#include <vector>

std::shared_mutex s_mtx;
int count;

void foo(int id){
    if(id & 1){
        s_mtx.lock(); // shared_mutex 作为独占锁使用
        count ++ ;
        std::cout << "thread id = " << id;
        std::cout << ", count = " << count << "\n";
        s_mtx.unlock();
    }else{
        if(s_mtx.try_lock()){
            count ++ ;
            std::cout << "thread id = " << id;
            std::cout << ", count = " << count << "\n";
            s_mtx.unlock();
        }
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