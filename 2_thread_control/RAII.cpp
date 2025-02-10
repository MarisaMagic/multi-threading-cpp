#include <iostream>
#include <thread>

class scoped_thread{
	std::thread t;
public:
	// 构造函数，采用移动语义（防止隐式转换）
	explicit scoped_thread(std::thread t_): t(std::move(t_)){
		if(!t.joinable()){
			throw std::logic_error("thread is unjoinable.");
		}
	}
	// 析构函数
	~scoped_thread(){
		if(t.joinable()){
			t.join();  // 析构时自动 join
		}
	}
	// 禁用拷贝构造函数
	scoped_thread(scoped_thread const&) = delete;
	// 禁用拷贝赋值函数
	scoped_thread& operator=(scoped_thread const&) = delete;
};

// 线程接口函数
void foo(int cnt){
	for(int i = 1; i <= cnt; i ++ ){
		std::cout << "Kirisame Marisa." << i << "\n";
	}
}

int main(){
	// scoped_thread st(std::thread{}); // ERROR 抛出异常
	scoped_thread st(std::thread(foo, 30));

	// 主线程中模拟做些其他事情
	for(int i = 1; i <= 3; i ++ ){
		std::cout << "Alice Margatroid. " << i << "\n";
	}

	return 0;
}