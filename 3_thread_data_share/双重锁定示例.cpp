#include <iostream>
#include <thread>
#include <mutex>
#include <vector>

std::mutex mtx;
bool flag;
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

int main(){
    std::vector<std::thread> threads(30);
    for(auto& t : threads) t = std::thread(foo);
    for(auto& t : threads) t.join();

    return 0;
}