#include <iostream>
#include <atomic>
#include <thread>
#include <chrono>

// 两个原子变量
std::atomic<int> atm1(0);
std::atomic<int> atm2(0);

void foo1(){
    // 循环多次进行存储操作
    for(int i = 0; i < 10000; i ++ ){
        atm1.store(i, std::memory_order_release);
        atm2.store(i, std::memory_order_release);
        // 短暂停顿，模拟实际中可能存在的操作间隙等情况
        std::this_thread::sleep_for(std::chrono::microseconds(1));
    }
}

void foo2(){
    // 循环多次进行加载操作，增加出现不同步情况的机会
    for(int i = 0; i < 10000; i ++ ){
        int val1 = atm1.load(std::memory_order_acquire);
        int val2 = atm2.load(std::memory_order_acquire);
        // 检查是否出现不一致的情况，两个值不符合预期的对应关系
        if(val1 != val2){
            std::cout << "Inconsistent values found: ";
            std::cout << "value1 = " << val1 << ", value2 = " << val2 << "\n";
        }
    }
}

int main(){
    std::thread t1(foo1);
    std::thread t2(foo2);

    t1.join();
    t2.join();

    return 0;
}