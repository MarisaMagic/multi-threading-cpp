#include <iostream>
#include <thread>

void foo(){
    std::cout << "foo(): thread starts." << "\n";
    // 模拟做一些事情
    std::cout << "foo(): MarisaMagic is doing sth." << "\n";
    std::this_thread::sleep_for(std::chrono::seconds(1)); 
    std::cout << "foo(): thread ends." << "\n";
}

int main(){
    std::cout << "main(): thread starts." << "\n";

    std::thread t(foo);
    t.detach();  // 主线程不等待子线程运行结束 

    std::cout << "main(): thread ends." << "\n";

    return 0;
}