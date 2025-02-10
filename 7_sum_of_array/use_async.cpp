#include <iostream>
#include <future>
#include <vector>
#include <numeric>
#include <chrono>

const int NUM_THREADS = 24; // 线程数量
long long arr_sum; // 共享数据变量，数组之和

// 异步任务执行函数，返回结果
long long get_sum(const std::vector<int>& arr, int start, int end){
    long long cur_sum = 0;
    for(size_t i = start; i < end; i ++ ){
        cur_sum += arr[i];
    }
    return cur_sum;
}

int main(){
    // 数组数据输入
    const int n = 1e9;
    std::vector<int> arr(n);
    std::iota(begin(arr), end(arr), 0);

    // 计算起始时间点
    auto start_time = std::chrono::high_resolution_clock::now();

    // 创建多个异步任务
    const int step = n / NUM_THREADS; // 每个线程计算的块大小
    std::vector<std::future<long long>> futures(NUM_THREADS);
    for(int i = 0; i < NUM_THREADS; i ++ ){
        int start = i * step;
        int end = (i == NUM_THREADS - 1) ? n : (i + 1) * step;
        // 创建异步任务自动创建线程执行，系统看情况选择执行策略
        futures[i] = std::async(get_sum, std::ref(arr), start, end);
    }

    // 等待每个数组子段的结果，累加
    for(auto& res : futures){
        arr_sum += res.get(); // 如果没计算完会阻塞主线程
    }

    // 计算终止时间点
    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> dura = end_time - start_time;
    std::cout << "spend time = " << dura.count() << "ms." << "\n";
    
    // 输出数组之和
    std::cout << "finally the sum = " << arr_sum << "\n";

    return 0;
}