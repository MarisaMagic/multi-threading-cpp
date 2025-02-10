#include <iostream>
#include <thread>

void func(){
	std::thread::id this_id = std::this_thread::get_id();
	std::cout << "thread id: " << this_id << "\n";
	std::this_thread::sleep_for(std::chrono::milliseconds(500));
}

int main(){
	std::thread t(func);
	std::thread t1(func); // 自动join的线程 C++20引入
	t.join();
	t1.join();
		
	return 0;
}