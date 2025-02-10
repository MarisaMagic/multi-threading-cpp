#include <iostream>
#include <vector>
#include <numeric>
#include <chrono>

long long arr_sum; // 共享数据变量，数组之和

// 数组求和函数
void get_sum(const std::vector<int>& arr){
    for(size_t i = 0; i < arr.size(); i ++ ){
        arr_sum += arr[i];
    }
}

int main(){
    // 数组数据
    const int n = 1e9;
    std::vector<int> arr(n);
    std::iota(begin(arr), end(arr), 0);

    // 计算起始时间点
    auto start_time = std::chrono::high_resolution_clock::now();

    // 数组求和
    get_sum(arr);

    // 计算终止时间点
    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> dura = end_time - start_time;
    std::cout << "spend time = " << dura.count() << "ms." << "\n";
    
    // 输出数组之和
    std::cout << "finally the sum = " << arr_sum << "\n";

    return 0;
}