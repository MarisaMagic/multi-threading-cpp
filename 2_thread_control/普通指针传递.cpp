#include <iostream>
#include <thread>

// 线程接口函数
void foo(int *p){ 
    *p += 1; // 注意 *p ++ 是先 p ++ 再取值。*p += 1 是先取值再 += 1
    std::cout << "foo(): " << *p << " Marisa" << "\n";
}

int main(){
    int num = 520;

    // 传递指针作为参数
    std::thread t(foo, &num); 
    t.join();

    // 成功修改了原始数据
    std::cout << "main(): " << num << " Marisa" << "\n"; 

    return 0;
}