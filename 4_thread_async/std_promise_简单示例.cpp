#include <iostream>
#include <thread>
#include <future>

void foo(std::promise<int>& pro) {
    int result = 1314;
    pro.set_value(result); // 设置我们想要的结果
}

int main() {
    std::promise<int> pro; // 创建 promise 对象
    std::future<int> res = pro.get_future(); // 获取 pro 关联的 future 对象
    std::thread t(foo, std::ref(pro)); // 开启子线程传入 pro

    // 调用 future::get() 等待获取结果
    int data = res.get();
    std::cout << "finally the res = " << data << std::endl;

    t.join();

    return 0;
}