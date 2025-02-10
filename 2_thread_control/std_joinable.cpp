#include <iostream>
#include <thread>

void foo(){ return; }

int main(){ 
    std::thread t1; // 默认构造thread对象，未关联任何函数
    std::cout << t1.joinable() << "\n";

    std::thread t2(foo);
    t2.join(); // 已经join
    std::cout << t2.joinable() << "\n";

    std::thread t3(foo);
    t3.detach(); // 已经detach
    std::cout << t3.joinable() << "\n"; 


    std::thread t(foo);
    if(t.joinable()){ // 检查是否可以进行join或detach
        t.join(); 
    }

    return 0;
}