#include <iostream>
#include <thread>

class MyClass{
	int val;
public:
    // 构造函数
	MyClass(int n): val(n){
		std::cout << "build function thread id: " << std::this_thread::get_id() << "\n";
	}

    // 拷贝构造函数
	MyClass(const MyClass& other){
		val = other.val;
		std::cout << "copy build function thread id: " << std::this_thread::get_id() << "\n";
	}

    // 析构函数
	~MyClass(){
		std::cout << "~function thread id: " << std::this_thread::get_id() << "\n";
	}

	void foo(int a){ 
		std::cout << "foo(): thread starts, thread id = " << std::this_thread::get_id() << "\n";
        for(int i = 1; i <= val; i ++ ){
            std::cout << "Marisa " << a << "\n";
        }
	}
};

int main(){
    MyClass obj(5);
    std::thread t(&MyClass::foo, &obj, 520);
    t.join();

    return 0;
}