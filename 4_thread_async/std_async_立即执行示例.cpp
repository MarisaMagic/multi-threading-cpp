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

    // 创建一个异步任务，std::launch::async 会立即在一个子线程中开始执行 foo()
    std::future<int> res = std::async(std::launch::async, foo, 3, 4);

    // 主线程 main() 中同时可以做一些其他事情
    std::cout << "main(): starts doing somethine else..." << "\n";
    std::this_thread::sleep_for(std::chrono::seconds(2));
    std::cout << "main(): ends doing something else..." << "\n";

    // (1) 在主线程中调用 get() 获取结果，如果 foo() 还未执行完，主线程会被阻塞，直到 foo() 执行结束得到结果
    // auto data = res.get();
    // std::cout << "finally get the res = " << data << "\n"; // 直到获取结果才会执行这一句
    // (2) 如果 foo() 还未执行完，主线程会被阻塞，但是不返回结果
    // res.wait(); 
    // (3) 如果前面调用 get() 或 wait()，这句会最后执行；否则，这句会先执行
    
    std::cout << "Kirisame Marisa." << "\n"; 

    return 0;
}