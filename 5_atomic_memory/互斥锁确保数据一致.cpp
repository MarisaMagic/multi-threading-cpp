#include <iostream>
#include <thread>
#include <mutex>

int cnt = 0;
std::mutex mtx;

void foo_use_lock(){
	for(int i = 1; i <= 1000000; i ++ ){
		std::lock_guard<std::mutex> lck(mtx); // 加锁
		cnt ++ ;
		cnt += 2;
	}
}

int main(){
	std::thread t1(foo_use_lock);
	std::thread t2(foo_use_lock);
	t1.join();
	t2.join();

	std::cout << cnt << "\n";

	return 0;
}