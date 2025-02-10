#include <iostream>
#include <thread>
#include <mutex>
#include <atomic>

std::atomic<int> at_cnt = 0;

void foo_use_atomic(){
	for(int i = 1; i <= 1000000; i ++ ){
		at_cnt ++ ;  // 原子操作不会被打断，无需加锁
		at_cnt += 2;
		// at_cnt = at_cnt + 1; // 不是原子操作
	}
}

int main(){
	std::thread t1(foo_use_atomic);
	std::thread t2(foo_use_atomic);
	t1.join();
	t2.join();

	std::cout << at_cnt << "\n";

	return 0;
}