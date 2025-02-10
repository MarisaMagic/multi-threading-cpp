#include <iostream>
#include <thread>

void foo(int a, std::string s){
    std::cout << a << " " << s << "\n";
}

int main(){
    int num = 520;
    std::string str = "Marisa";

    // 传入函数指针 foo，以及函数参数
    std::thread t(foo, num, str);   
    t.join();

    // 也可以是临时变量
    std::thread t1(foo, 521, "Alice");
    t1.join();

    return 0;
}