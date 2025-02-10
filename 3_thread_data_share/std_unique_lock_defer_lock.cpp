#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <queue>

/*
	std::unique_lock在默认情况下和lock_guard一样，隐式加解锁
	
	std::unique_lock 的 defer_lock 参数是构造函数的一个选项，用于指示 std::unique_lock 在创建时不要立即尝试获取互斥量
	允许你在稍后的某个时间点显式地调用 lock() 方法来获取锁
	也允许在之后的某个时间显式地调用 unlock() 方法来释放锁
	而lock_guard是无法进行上述的两个操作的
*/

std::mutex mtx;

void foo(int id){
	std::unique_lock<std::mutex> lck(mtx, std::defer_lock);
	if(id & 1){
		lck.lock(); 	// 显式获得锁
		std::cout << "thread " << id << " acquired the lock." << "\n";
		lck.unlock();   // 显式释放锁
	}else{
		std::cout << "thread " << id << " did not acquire the lock." << "\n";
	}
}

void foo2(int id){
	std::unique_lock<std::mutex> lck(mtx, std::defer_lock);
	// try_lock 函数尝试获取与 std::unique_lock 关联的互斥量，但不会阻塞。
	// 如果互斥量当前未被其他线程持有，try_lock 会成功获取锁并返回 true；
	// 如果互斥量已被其他线程持有，try_lock 会立即返回 false，而不会阻塞当前线程。
	if(lck.try_lock()){
		std::cout << "thread " << id << " acquired the lock." << "\n";
	}else{
		std::cout << "thread " << id << " did not acquire the lock." << "\n";
	}
}

int main(){
	std::thread t1(foo2, 1);
	std::thread t2(foo2, 2);

	t1.join();
	t2.join();

	return 0;
}