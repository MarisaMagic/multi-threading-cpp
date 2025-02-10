#include <iostream>
#include <thread>
#include <memory>

void foo(std::shared_ptr<int> p) {
    *p += 1;
    std::cout << "foo(): " << *p << " Marisa" << "\n";
}

int main() {
    // shared_ptr 智能指针
    std::shared_ptr<int> ptr = std::make_shared<int>(520);

    // 传递 shared_ptr 智能指针
    std::thread t(foo, ptr);
    t.join();

    // 成功修改了原始数据
    std::cout << "main(): " << *ptr << " Marisa" << "\n";

    return 0;
}