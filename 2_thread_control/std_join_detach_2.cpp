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
    t.detach();

    try{
        t.join(); // 不可多次detach()（再次join()也不可行）
    }catch(...){
        std::cout << "ERROR: multiple detach." << "\n";
    }

    std::cout << "main(): thread ends." << "\n";

    return 0;
}