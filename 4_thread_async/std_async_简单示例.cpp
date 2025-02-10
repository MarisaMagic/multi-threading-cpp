#include <iostream>
#include <future>
#include <thread>

int foo(int a, int b){
    std::cout << "foo(): thread id = " << std::this_thread::get_id() << "\n";
    return a + b;
}

int main(){
    std::cout << "main(): thread id = " << std::this_thread::get_id() << "\n";
    std::future<int> res = std::async(std::launch::deferred, foo, 1, 2);
    auto data = res.get();    
    std::cout << "finally get the res = " << data << "\n";

    return 0;
}