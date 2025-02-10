#include <iostream>
#include <thread>
#include <mutex>
#include <vector>

/*
	在多线程中，有一种场景是某个任务只需要执行一次，
	可以用C++11中的std::call_once函数配合std::once_flag来实现。多个线程同时调用某个函数，
	std::call_once可以保证多个线程对该函数只调用一次。
*/

std::once_flag flag;

void foo(){
	std::call_once(flag, [](){
		std::cout << "Kirisame Marisa." << "\n";
	});
}

int main(){
	std::vector<std::thread> threads;
	for(int i = 0; i < 5; i ++ ) threads.push_back(std::thread(foo));

	for(auto &t : threads) t.join();

	return 0;
}