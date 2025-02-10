#include <iostream>
#include <thread>
#include <memory>

void foo(std::unique_ptr<int> p) {
    *p += 1;
    std::cout << "foo(): " << *p << " Marisa" << "\n";
}

int main() {
    // unique_ptr 智能指针
    std::unique_ptr<int> ptr = std::make_unique<int>(520);

    // 传递 unique_ptr 智能指针
    std::thread t(foo, std::move(ptr));
    t.join();

    // 成功修改了原始数据
    // std::cout << "main(): " << *ptr << " Marisa" << "\n";

    return 0;
}