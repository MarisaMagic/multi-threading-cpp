#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <queue>

/*
	std::lock_guard类模板可以直接取代lock()和unlock().
	同一个线程中，std::lock_guard和lock/unlock()不能混用，如果用了lock_guard，就禁止使用lock()和unlock()

	std::lock_guard:
	在定义对象的位置，lock_guard 对象对互斥量进行一个lock();
	在对象析构的位置（通常是return），lock_guard 对象对互斥量进行一个unlock()
*/

class MyClass{
	std::queue<int> msg_que; // 存储用户命令的消息队列
	std::mutex mtx;     // 互斥量
public:
	void in_msg_que(int num){
		for(int i = 0; i < num; i ++ ){
			std::cout << "in_msg_que() running, push data: " << i << "\n";
			// 代替 lock(), unlock()
			std::lock_guard<std::mutex> lck(mtx); 
			msg_que.push(i);  // 放入消息队列
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
		// 代替 lock(), unlock()
		std::lock_guard<std::mutex> lck(mtx); 
		if(msg_que.empty()){
			// 如果为空，直接返回false。lck 对象析构，自动进行 unlock()
			return false; 
		}
		command = msg_que.front(); // 从队首取出指令
		msg_que.pop();
		return true; // 返回true，自动 unlock()
	}
};

int main(){
	MyClass a;
	std::thread out_msg_t(&MyClass::out_msg_que, &a, 100000); 
	std::thread in_msg_t(&MyClass::in_msg_que, &a, 100000);

	out_msg_t.join();
	in_msg_t.join();

	return 0;
}