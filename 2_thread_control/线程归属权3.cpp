#include <iostream>
#include <thread>

void foo1(){
	std::cout << "Kirisame Marisa" << "\n";
}

void foo2(){
	std::cout << "Alice Margatroid" << "\n";
}

int main(){
	std::thread t1(foo1); 		   // t1 关联线程函数 foo1
	std::thread t2(std::move(t1)); // t1 的线程所有权转移给 t2，t1 变成空线程，t2 关联 foo1
	t1 = std::thread(foo2);        // t1 关联线程函数 foo2

	// 此时两个线程都是 joinable 的
	std::cout << "t1 is joinable: " << t1.joinable() << "\n";
	std::cout << "t2 is joinable: " << t2.joinable() << "\n";

	std::thread t3;
	t3 = std::move(t2);   // t2 的线程所有权转移给 t3，t2 变成空线程，t3 关联 foo1
	// t1 = std::move(t3);  // ERROR
	// t1 拥有一个线程的所有权（关联 foo2）且还未执行结束（还未 join 或 detach）
	// 一个 std::thread 同一时间只能拥有一个线程的所有权

	// t2 没有关联任何函数，是 unjoinable 的
	t1.join();
	t3.join();

	return 0;
}