#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <queue>


class MyClass{
	std::queue<int> msg_que; // 存储用户命令的消息队列
	std::timed_mutex t_mtx;      // 带超时的独占互斥量
    // std::timed_mutex
        // try_lock_for():  
            // 等待一段时间，如果拿到锁或者等待超时后未拿到锁，流程继续
        // try_lock_until():
            // 等到未来的一个时间点，未到此时间点的这段时间内若拿到锁，流程继续 
            // 若在时间点到了，未拿到锁，流程也继续
public:
	void in_msg_que(int num){
		for(int i = 0; i < num; i ++ ){
			std::cout << "in_msg_que() running, push data: " << i << "\n";
			std::chrono::milliseconds timeout(100); // 超时设置为100毫秒
            if(t_mtx.try_lock_for(timeout)){

            // 等待时间点，当前时间之后的100毫秒，与上面的条件本质一样
            // if(t_mtx.tey_lock_until(std::chrono::steady_clock::now() + timeout)){
                
                // 在100毫秒之内成功拿到锁
                msg_que.push(i); // 放入消息队列
                t_mtx.unlock();  // 解锁
            }else{
                // 超过100毫秒超时，未拿到锁
                std::chrono::microseconds sleeptime(100);
                std::this_thread::sleep_for(sleeptime);
            }
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
        t_mtx.lock();
        std::chrono::microseconds sleeptime(1000);
        std::this_thread::sleep_for(sleeptime);
		if(msg_que.empty()){   // 如果为空，直接解锁返回false
			t_mtx.unlock();
            return false;
		}
		command = msg_que.front(); // 从队首取出指令
		msg_que.pop();
        t_mtx.unlock();
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