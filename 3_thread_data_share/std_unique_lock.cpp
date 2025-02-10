#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <queue>

class MyClass{
	std::queue<int> msg_que; // 存储用户命令的消息队列
	std::mutex mtx; // 互斥量
public:
	void in_msg_que(int num){
		for(int i = 0; i < num; i ++ ){
			std::cout << "in_msg_que() running, push data: " << i << "\n";
			// 使用 try_to_lock 参数，尝试加锁
			std::unique_lock<std::mutex> lck(mtx, std::try_to_lock);
			if(lck.owns_lock()){
				// 尝试后成功拿到了锁
				msg_que.push(i); 
			}else{
				// 没有成功拿到锁，直接返回
				std::cout << "in_msg_que() running, but cannot get lock: " << i << "\n";
			}
		}
	}

	void out_msg_que(int num){
		int command = 0;
		for(int i = 0; i < num; i ++ ){
			if(pop_command(command)){
				// 消息队列不为空，处理取出的指令
				std::cout << "out_msg_que() running, command is: " << command << "\n";
			}else{
				// 消息队列为空
				std::cout << "out_msg_que() running, queue is empty: " << i << "\n";
			}
		}
	}

	bool pop_command(int& command){
		std::unique_lock<std::mutex> lck(mtx); // 使用 unique_lock 实现自动加锁、解锁
		if(msg_que.empty()) return false; // 如果为空，直接返回 false
		command = msg_que.front(); // 从队首取出指令
		msg_que.pop();
		return true;
	}
};

int main(){
	MyClass obj;
	std::thread out_msg_t(&MyClass::out_msg_que, &obj, 10000); 
	std::thread in_msg_t(&MyClass::in_msg_que, &obj, 10000);

	out_msg_t.join();
	in_msg_t.join();

	return 0;
}