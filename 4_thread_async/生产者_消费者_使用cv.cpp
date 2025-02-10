#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <chrono>

std::mutex mtx; // 互斥量
std::condition_variable cv; // 条件变量
std::queue<int> buffer; // 共享缓冲区
const int BUFFER_SIZE = 10; // 缓冲区大小
const int PRODUCER_NUM = 2; // 生产者个数
const int CONSUMER_NUM = 5; // 消费者个数

// 生产者函数
void producer(int id, int data_num){
    for(int i = 1; i <= data_num; i ++ ){
        std::unique_lock<std::mutex> lck(mtx);
        // 等待 buffer 有空间可放入数据（循环检查，防止虚假唤醒）
        while(buffer.size() >= BUFFER_SIZE){
            cv.wait(lck); // 默认 false，解锁并进入等待
        }
        buffer.push(i);
        std::cout << "producer " << id << " produced data: " << i << "\n";
        cv.notify_all();  // 唤醒等待状态的消费者线程
    }
}

// 消费者函数
void consumer(int id){
    while(true){
        std::unique_lock<std::mutex> lck(mtx);
        // 等待 buffer 中有数据可消费（循环检查，防止虚假唤醒）
        while(buffer.empty()){
            cv.wait(lck); // 默认 false，解锁并进入等待
        }
        int data = buffer.front();
        buffer.pop();
        std::cout << "consumer " << id << " consumed data: " << data << "\n";
        lck.unlock();     // 提前解锁
        cv.notify_all();  // 唤醒等待状态的生产者线程
        // 模拟与 cv 无关的数据操作，放在解锁和通知之后
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

int main(){
    std::vector<std::thread> producers(PRODUCER_NUM);
    std::vector<std::thread> consumers(CONSUMER_NUM);
    for(size_t i = 0; i < producers.size(); i ++ ){
        producers[i] = std::thread(producer, i + 1, 100);
    }
    for(size_t i = 0; i < consumers.size(); i ++ ){
        consumers[i] = std::thread(consumer, i + 1);
    }
    for(auto& t : producers) t.join();
    for(auto& t : consumers) t.join();

    return 0;
}