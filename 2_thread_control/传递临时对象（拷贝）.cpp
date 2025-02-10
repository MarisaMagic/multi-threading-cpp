#include <iostream>
#include <thread>
#include <chrono>

class MyClass {
    int val;
public:
    // 构造函数
    MyClass(int n) : val(n){
        std::cout << "Build() thread id: " << std::this_thread::get_id() << "\n";
        std::cout << "Build function called" << "\n";
    } 

    // 析构函数
    ~MyClass(){
        std::cout << "~() thread id: " << std::this_thread::get_id() << "\n";
        std::cout << "~ function called" << "\n";
    }

    // 拷贝构造函数
    MyClass(const MyClass& other){ 
        val = other.val;
        std::cout << "Copy() thread id: " << std::this_thread::get_id() << "\n";
        std::cout << "Copy function called" << "\n";
    }

    // 线程接口函数
    void print_val(){
        std::cout << "print_val() thread id: " << std::this_thread::get_id() << "\n";
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::cout << val << " Marisa" << "\n";
    }
};

int main(){
    std::cout << "main() thread id: " << std::this_thread::get_id() << "\n";

    // 传递一个临时 MyClass 对象（通过类成员函数指针创建线程）
    std::thread t(&MyClass::print_val, MyClass(520)); 
    t.join();

    return 0;
}