#include <iostream>
#include <thread>
#include <mutex>

std::mutex mtx; // 互斥量
int count; 

void foo(int num){
    mtx.lock();
    for(int i = 0; i < num; i ++ ){
        count ++ ;
    }
    mtx.unlock();
}

int main(){
    std::thread t1(foo, 100000);
    std::thread t2(foo, 100000);
    t1.join();
    t2.join();

    std::cout << "final value: " << count << "\n";

    return 0;
}