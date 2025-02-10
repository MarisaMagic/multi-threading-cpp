#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <queue>
#include <windows.h>

#define __WINDOWSJQ_  // windows 多线程开关 

// 用于自动释放Windows多线程下的临界区，防止遗忘离开临界区
// 类似于C++ std::lock_guard<std::mutex>
#ifdef __WINDOWSJQ_
class WinLockGuard{
private:
	CRITICAL_SECTION *p_critical;
public:
	WinLockGuard(CRITICAL_SECTION *p_tmp){ // 构造函数
		p_critical = p_tmp;
		EnterCriticalSection(p_critical);
	}
	~WinLockGuard(){ // 析构函数，离开临界区
		LeaveCriticalSection(p_critical);
	}
};
#endif

class MyClass{
private:
	std::queue<int> msg_que; // 存储用户命令的消息队列
	
#ifdef __WINDOWSJQ_
    // windows 多线程互斥
    CRITICAL_SECTION winsec; // windows中的临界区，类似于C++ mutex
#else
    std::mutex mtx;   // 互斥量
#endif

public:
    MyClass(){
    #ifdef __WINDOWSJQ_
        InitializeCriticalSection(&winsec); // 初始化临界区
    #endif
    }

	void in_msg_que(int num){
		for(int i = 0; i < num; i ++ ){
			std::cout << "in_msg_que() running, push data: " << i << "\n";
        #ifdef __WINDOWSJQ_
            // windows多线程操作
			// 重复进入临界区
            EnterCriticalSection(&winsec); // 进入临界区（加锁）
            EnterCriticalSection(&winsec); // 重复进入临界区（加锁）
			WinLockGuard lck(&winsec);     // 调用自己实现的windows版lockguard
            WinLockGuard lck2(&winsec);
			msg_que.push(i);
            LeaveCriticalSection(&winsec); // 离开临界区（解锁）
	        LeaveCriticalSection(&winsec); // 需要第二次离开临界区（解锁）
			// 如果少离开一次，会一直执行消息入队的线程函数
		#else
			// 在C++中不允许对同一个互斥量lock/unlock多次
			// C++ lock_guard 同理
            mtx.lock(); 
			msg_que.push(i);    // 放入消息队列
			mtx.unlock();  
        #endif     
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
    #ifdef __WINDOWSJQ_
        // windows 多线程操作
        EnterCriticalSection(&winsec);
        if(msg_que.empty()){
            LeaveCriticalSection(&winsec);
            return false;
        }
        command = msg_que.front();
        msg_que.pop();
        LeaveCriticalSection(&winsec);
        return true;
    #else
		mtx.lock();
		if(msg_que.empty()){
			mtx.unlock();
			return false; // 如果为空，直接返回false
		}
		command = msg_que.front(); // 从队首取出指令
		msg_que.pop();
		mtx.unlock();
		return true;
    #endif
	}
};

int main(){
	// windows多线程多次进入临界区实验
	MyClass obj_a;
	std::thread out_msg_t(&MyClass::out_msg_que, &obj_a, 7); // 通过类成员函数创建线程
	std::thread in_msg_t(&MyClass::in_msg_que, &obj_a, 5);

	out_msg_t.join();
	in_msg_t.join();

	return 0;
}