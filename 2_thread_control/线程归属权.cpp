#include <iostream>
#include <thread>

void foo(){
	std::cout << "Kirisame Marisa" << "\n";
}

int main(){
	std::thread t(foo);
    t.join();   // 等待线程执行完毕
    // t.detach(); // 不等待，放到后台运行，t的线程归属权被转移

	return 0;
}