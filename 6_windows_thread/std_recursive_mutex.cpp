#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <queue>


class MyClass{
	std::queue<int> msg_que; // 存储用户命令的消息队列
	std::mutex mtx;          // 独占互斥量
	std::recursive_mutex r_mtx; // 递归的独占互斥量
    // 递归锁recursive_mutex 可以循序同一个线程中对同一个互斥量多次加锁
    // recursive_mutex 也有 lock() 和 unlock
    // 当使用了递归锁时，考虑代码是否还有优化空间，
    // recursive_mutex 的效率显然会比较低，故考虑优化尽量避免使用
    // 递归次数通常有限制（爆栈），但一般不会出现此状况
public:
	void in_msg_que(int num){
		for(int i = 0; i < num; i ++ ){
			std::cout << "in_msg_que() running, push data: " << i << "\n";
			std::lock_guard<std::recursive_mutex> lck(r_mtx);
			test_func1();  // 之前已经加锁，内部又重复加锁，导致程序崩溃
			msg_que.push(i);   
		}
	}

	void test_func1(){
		std::lock_guard<std::recursive_mutex> lck(r_mtx);
		std::cout << "test_func1() do something else..." << "\n";
        test_func2();
	}

	void test_func2(){
		std::lock_guard<std::recursive_mutex> lck(r_mtx);
		std::cout << "test_func2() do something else..." << "\n";
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
		std::lock_guard<std::recursive_mutex> lck(r_mtx);
		if(msg_que.empty()){   // 如果为空，直接解锁返回false
			return false;
		}
		command = msg_que.front(); // 从队首取出指令
		msg_que.pop();
		return true;
	}
};

int main(){
	MyClass obj_a;
	std::thread out_msg_t(&MyClass::out_msg_que, &obj_a, 10000); // 通过类成员函数创建线程
	std::thread in_msg_t(&MyClass::in_msg_que, &obj_a, 10000);

	out_msg_t.join();
	in_msg_t.join();

	return 0;
}