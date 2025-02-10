/*
	死锁 (dead lock):
	实例: 魔理沙在雾雨魔法店等爱丽丝，爱丽丝不来就不出门；
		  爱丽丝在爱丽丝邸等魔理沙，魔理沙不来就不出门。
	
	死锁要求至少两把锁（两个互斥量）才会发生。
	现在有两个线程A和B，都需要同时锁住两个互斥量a和b。
	A先锁a再锁b，B先锁b再锁a
	两个线程执行时，都分别只锁住了一个互斥量，
	等待着另一个线程解锁再给另一个互斥量加锁。

	死锁的一般解决方案:
	(1) 保证两个线程上锁的次序一样
	(2) 每个线程必须一次锁上所有的锁（可用std::lock实现）
	(3) 采用额外的限制条件
*/

#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <queue>

/*
	使用std::lock()可以解锁上面那种由于加锁顺序问题造成的死锁
	std::lock(): 一次锁住两个或两个以上的互斥量，不能只锁一个
	从第一个互斥量开始尝试上锁，如果lock()成功，就继续尝试下一个互斥量；一旦有一个互斥量锁不上，立即释放已经锁住的所有互斥量，从第一个互斥量开始重新尝试
	因此，std::lock()要么成功锁住所有互斥量；要么一个也不锁，全部释放
	
	std::lock()加锁与参数顺序无关，需要搭配多个unlock()，顺序随意
*/

class node{
	std::queue<int> msg_que; // 存储用户命令的消息队列
	std::mutex my_mutex1, my_mutex2; // 互斥量
public:
	void in_msg_que(int num){
		for(int i = 0; i < num; i ++ ){
			std::cout << "in_msg_que() running, push data: " << i << "\n";
			std::lock(my_mutex1, my_mutex2);  // 一次性锁住两个互斥量
			msg_que.push(i);    // 放入消息队列
			my_mutex1.unlock(); // unlock顺序随意
			my_mutex2.unlock();  
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
		if(msg_que.empty()){
			my_mutex1.unlock();
			my_mutex2.unlock();
			return false; // 如果为空，直接返回false
		}
		command = msg_que.front(); // 从队首取出指令
		msg_que.pop();
		my_mutex1.unlock();
		my_mutex2.unlock();
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