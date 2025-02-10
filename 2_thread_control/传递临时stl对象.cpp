#include <iostream>
#include <vector>
#include <thread>

void foo(std::vector<int> v, std::string s){
    std::cout << "I love " << s << "\n";
    for(const auto &x : v) std::cout << x << " ";
    std::cout << "\n";
}

int main(){
    // 传递 stl 临时对象（优先调用移动构造函数）
    std::thread t(foo, std::vector<int>{520, 1314}, std::string("Marisa"));
    t.join();

    return 0;
}