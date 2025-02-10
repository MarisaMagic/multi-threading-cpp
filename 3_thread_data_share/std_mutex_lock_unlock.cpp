#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <queue>

/*
	互斥锁 (mutex):
	1. 保护共享数据: 某个线程将共享数据锁住，自己进行操作数据
	2. 互斥锁是一个类对象，多个线程尝试用lock()成员函数加锁时，只有一个能成功
	3. 要准确确定（加锁）保护的代码块。保护区域前加锁lock，保护区域后解锁unlock。

	特点:
	- **原子性**: 
		如果一个线程锁定了一个互斥量，没有其他线程在同一时间可以成功锁定这个互斥量
		不会被其他线程中断
	
	- **唯一性**: 
		如果一个线程锁定了一个互斥量，在它解除锁定之前，没有其他线程可以锁定这个互斥量
		其他尝试获得同一锁的线程都会被阻塞，直到当前锁被释放

	- **非繁忙等待**:
		当一个线程调用 lock() 方法时，如果锁已被其他线程持有，调用线程会被阻塞并进入等待状态。
		此时，该线程不会占用 CPU 资源，而是让出 CPU 给其他线程使用。避免了繁忙等待（忙等待）

	std::mutex:
	- std::mutex不允许拷贝构造，也不允许 move ，最初产生的 mutex 对象处于 unlocked 状态
	- lock(): 调用线程将锁住该互斥量
		- 如果该互斥量当前没有被锁住，则调用线程将该互斥量锁住，直到调用 unlock之前，该线程一直拥有该锁。
		- 如果当前互斥量被其他线程锁住，则当前的调用线程被阻塞住，避免繁忙等待
		- 如果当前互斥量被当前调用线程锁住，则会产生死锁
	- unlock(): 解锁，释放对互斥量的所有权
	- try_lock(): 尝试锁住互斥量，如果互斥量被其他线程占有，则当前线程也不会被阻塞
		- 如果当前互斥量被其他线程锁住，则当前调用线程返回 false，而并不会被阻塞掉。
*/

// 问题背景: 
// (1) 一个线程用来接受用户命令（数字表示），并把命令存入一个队列中
// (2) 另一个线程从队列中取命令，解析并执行操作
// 程序中共享数据 msg_que 同一时刻只允许一个进程使用，
// msg_que 是临界资源，要对访问临界资源的代码加锁

class MyClass{
	std::queue<int> msg_que; // 存储用户命令的消息队列
	std::mutex mtx;          // 互斥量
public:
	void in_msg_que(int num){
		for(int i = 0; i < num; i ++ ){
			std::cout << "in_msg_que() running, push data: " << i << "\n";
			mtx.lock();    // 加锁
			msg_que.push(i);    // 放入消息队列
			mtx.unlock();  // 解锁
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
		mtx.lock();
		if(msg_que.empty()){   // 如果为空，直接解锁返回false
			mtx.unlock();
			return false;
		}
		command = msg_que.front(); // 从队首取出指令
		msg_que.pop();
		mtx.unlock();
		return true;
	}
};

int main(){
	MyClass obj_a;
	std::thread out_msg_t(&MyClass::out_msg_que, &obj_a, 20); // 通过类成员函数创建线程
	std::thread in_msg_t(&MyClass::in_msg_que, &obj_a, 20);

	out_msg_t.join();
	in_msg_t.join();

	return 0;
}