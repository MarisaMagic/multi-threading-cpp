#include <iostream>
#include <thread>
#include <mutex>
#include <vector>

std::mutex mtx; // 互斥量
bool flag;      // 标记是否初始化
int val;        // 待初始化数据

void init(){
    std::lock_guard<std::mutex> lck(mtx); // 互斥锁
    if(!flag){ // 还未初始化
        std::cout << "value initializing ... " << "\n";
        val = 1;     // 初始化数据为 1
        std::cout << "value initialized as: " << val << '\n';
        flag = true; // 标记为已经初始化
    }
}

int main(){
    std::vector<std::thread> threads(10);
    for(auto& t : threads) t = std::thread(init);
    for(auto& t : threads) t.join();

    return 0;
}