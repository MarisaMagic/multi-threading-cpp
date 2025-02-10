#include <iostream>
#include <thread>
#include <mutex>
#include <vector>

std::mutex mtx;       // 互斥量
std::once_flag flag;  // 标记
int val;              // 共享数据

void init(){
    std::call_once(flag, [](){
        std::cout << "value initializing ... " << "\n";
        val = 1;  // 初始化为 1
        std::cout << "value initialized as: " << val << '\n';
    });
}

int main(){
    std::vector<std::thread> threads(5);
    for(auto& t : threads) t = std::thread(init);
    for(auto& t : threads) t.join();

    return 0;
}