#include <iostream>
#include <thread>
#include <mutex>
#include <atomic>
#include <condition_variable>
#include <queue>
#include <vector>
#include <chrono>

std::mutex mtx; // 互斥量
std::condition_variable cv; // 条件变量
std::queue<int> buffer; // 共享缓冲区
const int BUFFER_SIZE = 10; // 缓冲区大小
const int PRODUCER_NUM = 2; // 生产者个数
const int CONSUMER_NUM = 5; // 消费者个数
// std::atomic<int> produce_finished_count; // 可以用原子操作
int produce_finished_count; // 生产者完成计数

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
    {
        //当前生产者生成数据完毕，修改完成数据生成的生产者记数
        std::lock_guard<std::mutex> lck(mtx);
        produce_finished_count ++ ;
        if(produce_finished_count == PRODUCER_NUM){
            cv.notify_all(); // 唤醒等待的消费者线程（此时唤醒是为了通知完全无数据时生产者退出循环）
        }
    }
}

// 消费者函数
void consumer(int id){
    while(true){
        std::unique_lock<std::mutex> lck(mtx);
        // 等待 buffer 中有数据可消费（循环检查，防止虚假唤醒）
        while(buffer.empty() && produce_finished_count < PRODUCER_NUM){
            cv.wait(lck); // 默认 false，解锁并进入等待
        }
        // 上面的检查中，可能因为 produce_finished_count == PRODUCER_NUM 而退出
        // 同时 buffer 可能为空，说明没有新的数据再生产，这个时候退出循环
        if(buffer.empty() && produce_finished_count == PRODUCER_NUM){
            break;  // 退出循环
        }
        int data = buffer.front();
        buffer.pop();
        std::cout << "consumer " << id << " consumed data: " << data << "\n";
        lck.unlock();     // 提前解锁
        cv.notify_all();  // 唤醒等待状态的生产者线程
        // 模拟与 cv 无关的、无需互斥量的数据操作，放在解锁和唤醒之后
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