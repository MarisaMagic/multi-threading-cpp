#include <iostream>
#include <thread>
#include <mutex>
#include <vector>

std::mutex mtx;
int count;

void foo(){
    mtx.lock();
    // 锁的管理转移给 unique_lock，加上 std::adopt_lock
	std::unique_lock<std::mutex> lck(mtx, std::adopt_lock);
    count ++ ;
    std::cout << "thread id = " << std::this_thread::get_id();
    std::cout << ", count = " << count << "\n"; 
    // 不需要再调用 mtx.unlock() 进行解锁，会自动解锁
}

int main(){
	std::vector<std::thread> v(10);
    for(auto& t : v) t = std::thread(foo);
    for(auto& t : v) t.join();

    std::cout << "final count = " << count << "\n";

	return 0;
}