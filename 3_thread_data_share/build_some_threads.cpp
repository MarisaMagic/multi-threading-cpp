#include <iostream>
#include <thread>
#include <vector>

/* 多个线程的执行顺序是乱的，这和操作系统的调度机制有关 */

void func(int num){
	std::cout << "Kirisame Marisa: " << num << "\n";
	std::cout << "Alice Margatroid: " << num << "\n";
}

int main(){
	std::vector<std::thread> vt;
	for(int i = 0; i < 10; i ++ ){
		vt.emplace_back(std::thread(func, i));
	}

	for(auto &t : vt) t.join();

	return 0;
}