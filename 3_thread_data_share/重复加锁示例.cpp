#include <iostream>
#include <thread>
#include <mutex>
#include <vector>

std::mutex mtx;
int count;

void dfs(int k){
    if(k == 0) return;
    mtx.lock();     // 加锁
    count ++ ;
    std::cout << "thread id = " << std::this_thread::get_id();
    std::cout << ", count = " << count << "\n";
    dfs(k - 1);     // 递归
    mtx.unlock();   // 解锁
}

int main(){
    std::vector<std::thread> threads(3);
    for(auto& t : threads) t = std::thread(dfs, 5);
    for(auto& t : threads) t.join();

    std::cout << "final count = " << count << "\n";

    return 0;
}