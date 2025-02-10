#include <iostream>
#include <thread>
#include <future>

int foo(int a, int b){
    return a + b;
}

int main(){
    std::packaged_task<int(int, int)> task(foo); // 包装一个异步任务
    std::future<int> res = task.get_future(); // 获取关联的 future 对象

    // 创建一个子线程执行异步任务
    std::thread t(std::ref(task), 1, 2);
    t.join();

    // 通过 get() 获取结果
    auto data = res.get();
    std::cout << "finally get the res = " << data << "\n";

    return 0;
}