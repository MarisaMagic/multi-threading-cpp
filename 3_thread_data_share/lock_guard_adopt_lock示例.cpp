#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

std::mutex mtx;
int count;

void foo(){
    mtx.lock();
    std::lock_guard<std::mutex> lck(mtx, std::adopt_lock);
    count ++ ;
    std::cout << "thread id = " << std::this_thread::get_id();
    std::cout << ", count = " << count << "\n";
}

int main(){
    std::vector<std::thread> threads(15);
    for(auto& t : threads) t = std::thread(foo);
    for(auto& t : threads) t.join();

    return 0;
}