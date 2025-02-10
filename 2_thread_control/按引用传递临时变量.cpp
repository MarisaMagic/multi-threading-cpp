#include <iostream>
#include <thread>

// 线程接口函数
void foo(int& a, std::string s){ 
    std::cout << "foo(): " << a << " " << s << "\n";
}

int main(){
    // 按引用传递临时变量 
    // std::thread t(foo, std::ref(520 + 1), "Marisa"); // ERROR
    // t.join();

    return 0;
}