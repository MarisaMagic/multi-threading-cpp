#include <iostream>
#include <thread>
#include <future>
#include <chrono>

int foo(int a, int b){
    std::cout << "foo(): thread id = " << std::this_thread::get_id() << "\n";
    std::cout << "foo(): starts calculating the res..." << "\n";
    std::this_thread::sleep_for(std::chrono::seconds(4));
    std::cout << "foo(): ends calculating the res..." << "\n";
    return a + b;
}

int main(){
    std::cout << "main(): thread id = " << std::this_thread::get_id() << "\n";

    // 创建一个异步任务，std::launch::deferred 会延迟执行 foo()，直到某个调用 get() 或 wait()
    std::future<int> res = std::async(std::launch::deferred, foo, 3, 4);

    // 主线程 main() 中同时可以做一些其他事情
    std::cout << "main(): starts doing somethine else..." << "\n";
    std::this_thread::sleep_for(std::chrono::seconds(2));
    std::cout << "main(): ends doing something else..." << "\n";

    // (1) 在主线程中调用 get() 才开始执行 foo()，实际上是在主线程中执行，最后获取结果
    auto data = res.get();
    std::cout << "finally get the res = " << data << "\n"; // 直到获取结果才会执行这一句
    // (2) 在主线程中调用 get() 才开始执行 foo()，但最后不获取结果
    // res.wait(); 
    // (3) 如果前面不调用 get() 或 wait()，foo() 压根不会被执行，会直接输出这一句
    
    std::cout << "Kirisame Marisa." << "\n"; 

    return 0;
}