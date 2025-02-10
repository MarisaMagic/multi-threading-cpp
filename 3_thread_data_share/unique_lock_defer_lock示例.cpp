#include <iostream>
#include <thread>
#include <mutex>
#include <vector>

std::mutex mtx;
int count;

void foo(int id){
	std::unique_lock<std::mutex> lck(mtx, std::defer_lock);
	if(id & 1){
		lck.lock(); 	// 手动加锁
        count ++ ;
		std::cout << "thread " << id << " acquired the lock." << "\n";
	}else{
		std::cout << "thread " << id << " did not acquire the lock." << "\n";
	}
}

int main(){
	std::vector<std::thread> v(20);
    for(int i = 0; i < 20; i ++ ) v[i] = std::thread(foo, i);
    for(auto& t : v) t.join();

    std::cout << "final count = " << count << "\n";

	return 0;
}