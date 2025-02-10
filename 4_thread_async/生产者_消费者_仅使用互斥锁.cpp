#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <chrono>

std::mutex mtx;         // 互斥量
std::queue<int> buffer; // 共享缓冲区
const int buffer_size = 10; // 缓冲区大小限制

// 生产者函数
void producer(int data_num){
    for(int i = 1; i <= data_num; i ++ ){
        std::unique_lock<std::mutex> lck(mtx);
        if(buffer.size() < buffer_size){ // 可以放入数据
            buffer.push(i);
            std::cout << "Produced: " << i << "\n";
        }else{ // 缓冲区已满
            lck.unlock(); // 先解锁
            // 休息一段时间再尝试
            std::this_thread::sleep_for(std::chrono::milliseconds(100)); 
            i -- ; // 回退
        }
    }
}

// 消费者函数
void consumer(){
    while(true){
        std::unique_lock<std::mutex> lck(mtx);
        if(!buffer.empty()){ // 可以消费数据
            int data = buffer.front();
            buffer.pop();
            std::cout << "Consumed: " << data << "\n";
            lck.unlock(); // 提前解锁
            // 模拟与 cv 无关的数据操作
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }else{
            lck.unlock(); // 先解锁
            // 休息一段时间再尝试
            std::this_thread::sleep_for(std::chrono::milliseconds(100)); 
        }
    }
}

int main(){
    std::thread producer_thread(producer, 100);
    std::thread consumer_thread(consumer);
    producer_thread.join();
    consumer_thread.join();

    return 0;
}