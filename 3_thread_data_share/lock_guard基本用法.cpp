#include <iostream>
#include <thread>
#include <mutex>

std::mutex mtx; // 互斥量
int count;

void foo(int num){
    for(int i = 0; i < num; i ++ ){
        // lock_guard 代替 lock 和 unlock
        std::lock_guard<std::mutex> lck(mtx); // 自动进行 lock()  
        count ++ ;
        std::cout << "thread id = " << std::this_thread::get_id();
        std::cout << ", count = " << count << "\n";
        // 在定义域结束时，自动进行 unlock()
    }
}

int main(){
    std::thread t1(foo, 100000);
    std::thread t2(foo, 100000);
    t1.join();
    t2.join();

    std::cout << "final count = " << count << "\n";

    return 0;
}