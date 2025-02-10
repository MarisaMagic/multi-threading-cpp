#include <iostream>
#include <vector>
#include <thread>
#include <shared_mutex>
#include <chrono>

#define take_nap(t) std::this_thread::sleep_for(std::chrono::milliseconds(t))

/*
	std::shared_mutex (C++17 引入)
	- 支持多个读取者同时访问共享资源
	- 当有写入操作时，会独占锁，仅有一个线程可以占有

	写锁:
	lock() 和 unlock(): 获取和释放独占锁（写锁）。
	try_lock(): 尝试获取独占锁，如果无法立即获得，则返回 false

	读锁:
	lock_shared() 和 unlock_shared(): 获取和释放共享锁（读锁）。
	try_lock_shared(): 尝试获取共享锁，如果无法立即获得，则返回 false。
	
	std::shared_lock用于管理std::shared_mutex 的共享锁（读锁)
*/

class node{
	std::shared_mutex mtx;
	int val = 0;
public:
	void read_data(int id){
		std::shared_lock<std::shared_mutex> lck(mtx); // 共享锁（读锁）
		std::cout << "Reader " << id << " reads value: " << val << "\n";
	}

	void write_data(int id, int n_val){
		std::unique_lock<std::shared_mutex> lck(mtx); // 独占锁（写锁）
		val = n_val;
		std::cout << "Writer " << id << " sets value to: " << val << "\n";
	}
};

void read(node &data, int id){
	for(int i = 0; i < 100; i ++ ){
		data.read_data(id);
		take_nap(100);
	}
}

void write(node &data, int id){
	for(int i = 0; i < 10; i ++ ){
		data.write_data(id, i);
		take_nap(1000);
	}
}

/*
	read_data 方法在读取数据前会获取一个共享锁，
	而 write_data 方法则在写入数据前获取一个独占锁。
	这样可以允许多个读取线程同时访问数据，
	但是一旦有写入线程尝试修改数据，所有试图获取共享锁的线程都会被阻塞，直到写入完成
*/

int main(){
	node a;

	std::vector<std::thread> readers;
	for(int i = 0; i < 5; i ++ ) readers.emplace_back(read, std::ref(a), i);

	std::thread writer(write, std::ref(a), 0);

	for(auto &t : readers) t.join();
	writer.join();

	return 0;
}