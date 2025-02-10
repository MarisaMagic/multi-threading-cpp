#include <iostream>
#include <thread>
#include <atomic>
#include <vector>
#include <numeric>
#include <chrono>

const int NUM_THREADS = 24;
std::atomic<long long> arr_sum; // 共享数据变量，数组之和

// 线程接口函数
void get_sum(const std::vector<int>& arr, int start, int end){
    long long cur_sum = 0;
    for(size_t i = start; i < end; i ++ ){
        cur_sum += arr[i];
    }
    arr_sum += cur_sum; // 原子操作，无需加锁
}

int main(){
    // 数组数据输入
    const int n = 1e9;
    std::vector<int> arr(n);
    std::iota(begin(arr), end(arr), 0);

    // 计算起始时间点
    auto start_time = std::chrono::high_resolution_clock::now();

    // 创建多个线程
    const int step = n / NUM_THREADS; // 每个线程计算的块大小
    std::vector<std::thread> threads(NUM_THREADS);
    for(int i = 0; i < NUM_THREADS; i ++ ){
        int start = i * step;
        int end = (i == NUM_THREADS - 1) ? n : (i + 1) * step;
        threads[i] = std::thread(get_sum, std::ref(arr), start, end);
    }

    // 等待所有线程完成
    for(auto& t : threads) t.join();

    // 计算终止时间点
    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> dura = end_time - start_time;
    std::cout << "spend time = " << dura.count() << "ms." << "\n";
    
    // 输出数组之和
    std::cout << "finally the sum = " << arr_sum << "\n";

    return 0;
}