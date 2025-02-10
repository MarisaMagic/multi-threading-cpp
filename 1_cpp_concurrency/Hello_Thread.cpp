#include <iostream>
#include <thread>

void foo(){
	std::cout << "Hello, this is MarisaMagic." << "\n";
}

int main(){  // 主线程从 main() 开始执行
	std::thread t(foo); // 新的子线程 t 从 foo() 开始执行
	t.join();           // 等待 t 线程执行完成

	return 0;
}