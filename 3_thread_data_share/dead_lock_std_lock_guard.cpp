#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <queue>

/*
	std::adopt_lock是一个结构体，它是std::lock_guard类的构造函数的一个可选参数
	在构造 lock_guard 对象的时候，告诉它其互斥量参数已经lock()过了，不需要 lock_guard对象再帮他 lock 了
	使用方法：std::lock_guard<std::mutex> threadGuard(my_mutex, std::adopt_lock);

	使用std::lock() 和带std::adopt_lock的std::lock_guard进行程序改写
	- std::lock()负责一次lock()多个信号量，解决死锁问题；
	- std::lock_guard负责unlock()信号量，避免忘记unlock()
	- std::adopt_lock用来避免重复lock()（省去lock_guard的重复lock操作）
*/

class node{
	std::queue<int> msg_que; // 存储用户命令的消息队列
	std::mutex my_mutex1, my_mutex2; // 互斥量
public:
	void in_msg_que(int num){
		for(int i = 0; i < num; i ++ ){
			std::cout << "in_msg_que() running, push data: " << i << "\n";
			
			std::lock(my_mutex1, my_mutex2); // 一次性锁住两个互斥量
			// 用lock_guard来unlock，adopt_lock避免重复lock操作
			std::lock_guard<std::mutex> t_guard1(my_mutex1, std::adopt_lock);
			std::lock_guard<std::mutex> t_guard2(my_mutex2, std::adopt_lock);

			msg_que.push(i);    // 放入消息队列 
		}
	}

	void out_msg_que(int num){
		for(int i = 0; i < num; i ++ ){
			int command;  // 取出的指令
			if(pop_command(command)){
				// 消息队列不为空，处理取出的指令
				std::cout << "out_msg_que() running, command is: " << command << "\n";
			}else{
				// 消息队列为空
				std::cout << "out_msg_que() running, queue is empty: " << i << "\n";
			}
		}
	}

	bool pop_command(int &command){
		std::lock(my_mutex1, my_mutex2);
		std::lock_guard<std::mutex> t_guard1(my_mutex1, std::adopt_lock);
		std::lock_guard<std::mutex> t_guard2(my_mutex2, std::adopt_lock);

		if(msg_que.empty()) return false; // 如果为空，直接返回false
		command = msg_que.front(); // 从队首取出指令
		msg_que.pop();
		return true;
	}
};

int main(){
	node a;
	std::thread out_msg_t(&node::out_msg_que, &a, 7); // 通过类成员函数创建线程
	std::thread in_msg_t(&node::in_msg_que, &a, 5);

	out_msg_t.join();
	in_msg_t.join();

	return 0;
}