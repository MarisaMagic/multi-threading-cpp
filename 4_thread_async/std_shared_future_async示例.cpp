#include <iostream>
#include <thread>
#include <future>
#include <chrono>

int foo(int a, int b){
	std::cout << "foo(): starts running, thread id = ";
    std::cout << std::this_thread::get_id() << "\n";
    
    // 模拟数据操作所需时间
    std::this_thread::sleep_for(std::chrono::seconds(2));

    std::cout << "foo(): ends running, thread id = ";
    std::cout << std::this_thread::get_id() << "\n";

    return a + b;
}

int main(){
	std::cout << "main(): starts running, thread id = ";
    std::cout << std::this_thread::get_id() << "\n";

    // 创建一个异步任务，std::launch::async 会立即在一个子线程中开始执行 foo()
    std::future<int> res = std::async(std::launch::async, foo, 3, 4);
    std::shared_future<int> shared_res = res.share();

    // 在主线程中调用 get() 获取结果，如果 foo() 还未执行完，主线程会被阻塞，直到 foo() 执行结束得到结果
    auto data = shared_res.get();
    std::cout << "main(): get the res = " << data << "\n"; 
 
    // 开启一个新的子线程，等待获取结果
    std::thread t([&](){
        std::cout << "lambda(): starts running, thread id = ";
        std::cout << std::this_thread::get_id() << "\n";

        auto data = shared_res.get();
        std::cout << "lambda(): get the res = " << data << "\n";

        std::cout << "lambda(): ends running, thread id = ";
        std::cout << std::this_thread::get_id() << "\n";
    });

    // 主线程 main() 中同时可以做一些其他事情
    std::cout << "main(): starts doing somethine else..." << "\n";
    std::this_thread::sleep_for(std::chrono::seconds(2));
    std::cout << "main(): ends doing something else..." << "\n";

    t.join();

	std::cout << "main(): ends running, thread id = ";
    std::cout << std::this_thread::get_id() << "\n";

    return 0;
}