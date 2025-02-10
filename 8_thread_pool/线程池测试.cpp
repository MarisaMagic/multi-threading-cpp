#include <iostream>
#include <future>
#include <vector>
#include "Thread_Pool.cpp"
using namespace std;

int main(){
    ThreadPool pool(4); // 创建一个有 4 个线程的线程池

    // 提交一些任务
    vector<pair<future<int>, int>> results; // 存储 future 对象及任务编号
    for(int i = 0; i < 12; i ++ ){
        // 提交任务后，得到 std::future 对象
        auto res = pool.submit([](int x){
            cout << "Task " << x << ": thread id = " << this_thread::get_id() << "\n";
            return x * x;
        }, i);
        results.emplace_back(move(res), i); // std::future 不可拷贝
    }

    // 获取结果
    for(auto& [res, id] : results){
        cout << "Task " << id << ": result = " << res.get() << "\n";
    }

    return 0;
}