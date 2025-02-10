#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <queue>


class node{
	std::queue<int> msg_que; // 存储用户命令的消息队列
	std::mutex my_mutex;     // 互斥量
	std::condition_variable my_cond;  // 条件变量
public:
	void in_msg_que(int num){
		for(int i = 0; i < num; i ++ ){
			std::cout << "in_msg_que() running, push data: " << i << "\n";
			std::unique_lock<std::mutex> t_guard(my_mutex);
			msg_que.push(i);  // 数据入队

			my_cond.notify_one(); // 尝试把处于wait状态的读线程唤醒
			// my_cond.notify_all();
		}
	}

	void out_msg_que(int num){
		int command = 0;
		while(true){
			std::unique_lock<std::mutex> t_guard(my_mutex);

			// 条件变量的成员函数wait()用来等待一个条件（则等待一次，默认为false，唤醒一次后为true）
			// （1）如果第二个参数lambda表达式返回值为false，wait将解锁互斥量并堵塞
			// 		需等待其他某个线程调用notify_one()成员函数，此时再继续执行
			// （2）否则，继续执行
			//  等待到有其他线程调用notify_one将此线程唤醒后，此线程会重新不断尝试加锁
			//  如果获取不到，还是阻塞；如果获取到了锁，wait被唤醒继续执行，再次判断(1), (2)
			my_cond.wait(t_guard, [&]{ return !msg_que.empty(); });

			// 保证msg_que至少有一条数据
			command = msg_que.front();
			msg_que.pop();
			t_guard.unlock(); // 提前解锁
			std::cout << "out_msg_que() running, get data: " << command << "\n";
			
			/* 存在问题 */
			// 提前解锁的情况下，假如下面继续执行其他的操作（需要一定时间）
			// 如果此时写线程调用了notify_one()函数
			// 那么此时notify_one()就没有什么效果，没有唤醒线程
		}

		// for(int i = 0; i < num; i ++ ){
		// 	if(pop_command(command)){
		// 		// 消息队列不为空，处理取出的指令
		// 		std::cout << "out_msg_que() running, command is: " << command << "\n";
		// 	}else{
		// 		// 消息队列为空
		// 		std::cout << "out_msg_que() running, queue is empty: " << i << "\n";
		// 	}
		// }
	}

	// bool pop_command(int &command){
	// 	if(!msg_que.empty()){ // 改写 写法1: 双重锁定、双重检查（先检查一下队列是否有数据，如有再加锁）
	// 		std::unique_lock<std::mutex> t_guard(my_mutex);
	// 	    // 原本的写法（无双重检查），可能会造成加锁之后，没有数据
	// 	    // 本来互斥量可用于其他线程，进而造成资源浪费
	// 		if(!msg_que.empty()){
	// 			command = msg_que.front();
	// 			msg_que.pop();
	// 			return true;
	// 		}
	// 	}
	// 	return false;
	// }
};

int main(){
	// 条件变量std::condition_variable，wait()，notify_one(): 只会随机唤醒一个线程
	// 线程A: 等待一个条件满足
	// 线程B: 专门往消息队列里面放入消息（数据）
	// condition_variable 需要配合互斥量工作，用于等待一个条件达成

	// notify_all() 可以唤醒多个线程

	node a;
	std::thread out_msg_t1(&node::out_msg_que, &a, 10);
	std::thread out_msg_t2(&node::out_msg_que, &a, 10);
	std::thread in_msg_t(&node::in_msg_que, &a, 20);

	out_msg_t1.join();
	out_msg_t2.join();
	in_msg_t.join();

	return 0;
}