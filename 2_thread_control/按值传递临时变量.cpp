#include <iostream>
#include <thread>

// 线程接口函数
void foo(int a, std::string s){ 
    std::cout << "foo(): " << a << " " << s << "\n";
}

int main(){
    // 按值传递临时变量
    std::thread t(foo, 520, "Marisa"); 
    t.join();

    return 0;
}