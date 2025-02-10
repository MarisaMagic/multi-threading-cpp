#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>

#define take_nap(t) std::this_thread::sleep_for(std::chrono::milliseconds(t))

// 单例设计模式: 在整个项目中，有某个或某些特殊的类，只允许创建一个

std::mutex mtx;      // 互斥量
std::once_flag flag; // call_once标记 

class MyCAS{
	static MyCAS *m_instance; // 静态成员变量
	MyCAS() {} // 私有化构造函数
	static void get_instance_once(){
		take_nap(5000);
		m_instance = new MyCAS();
		static Recycle r;
	}
public:
	static MyCAS* get_instance(){
		/*
			提高效率:
			1) 如果 m_instance != NULL，表示m_instance一定创建过了
			2) 否则，不一定创建过（new）
		*/
		// if(m_instance == NULL){ // 双重锁定，避免每次调用时都进行unique_lock
		// 	std::unique_lock<std::mutex> lck(mtx); // 自动加锁解锁
		// 	if(m_instance == NULL){
		// 		m_instance = new MyCAS();
		// 		static Recycle r;
		// 	}
		// }
		std::call_once(flag, get_instance_once);

		// 若两个线程同时执行到call_once，其中一个线程成功执行
		// 另外一个线程等待

		return m_instance;
	}

	/*
		不能在MyCAS的析构函数进行delete操作，因为返回的是指针对象，
		指针对象进行delete之后，会继续调用析构函数
		此时会无限递归调用导致报错
	*/
	class Recycle{ // 类中套类，用于释放对象
	public:
		~Recycle(){ // return 0 之前调用析构函数释放对象
			if(MyCAS::m_instance){
				delete MyCAS::m_instance;
				MyCAS::m_instance = NULL;
			}
		}
	};

	void foo(){
		std::cout << "Kirisame Marisa." << "\n";
	}
};

// 静态成员变量初始化
MyCAS* MyCAS::m_instance = NULL;

// 线程入口函数
void build_thread(){
	std::cout << "thread begins" << "\n";
	MyCAS *p = MyCAS::get_instance();
	p->foo();
	std::cout << "thread ends" << "\n";
}

int main(){
	// MyCAS *p = MyCAS::get_instance(); // 创建一个对象
	// p->foo();

	/*
		在多线程中，不同线程执行时会导致问题:
		A线程还未执行m_instance创建时，切换B线程并继续执行
		因此，可以增加一个互斥量避免这种情况，因此起到一个互斥的效果
	*/

	std::thread t1(build_thread);
	std::thread t2(build_thread);

	t1.join();
	t2.join();

	/*
		std::call_once(), C++11 引入
		call_once保证函数foo()只被调用一次
		call_once具备互斥的能力，且消耗资源更少
		call_once需要结合标记std::once_flag使用
		call_once通过这个标记判断函数foo是否被调用过
	*/

	return 0;
}