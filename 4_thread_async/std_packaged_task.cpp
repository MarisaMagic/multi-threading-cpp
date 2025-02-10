#include <iostream>
#include <thread>
#include <chrono>
#include <future>

int foo(int a, int b){
	std::cout << "foo() starts, thread id = " << std::this_thread::get_id() << "\n";
	std::this_thread::sleep_for(std::chrono::milliseconds(2000));
	std::cout << "foo() ends, thread id = " << std::this_thread::get_id() << "\n";
	return a + b;
}

void run_in_thread(){
	std::packaged_task<int(int, int)> pt(foo); 	// 包装一个异步任务
	std::future<int> res = pt.get_future(); // 获取 pt 关联的 future 对象
	
	// 开启一个子线程，调用对象包装引用，启动异步任务
	std::thread t(std::ref(pt), 3, 4); 
	t.join();

	// 获取 packaged_task 的结果
	int data = res.get();
	std::cout << "run_in_thread: get the res = " << data << "\n";
}

void run_in_main(){
	std::packaged_task<int(int, int)> pt(foo); 	// 包装一个异步任务
	std::future<int> res = pt.get_future(); // 获取 pt 关联的 future 对象

	// 直接执行任务
	pt(1, 2); // 传入参数

	// 获取 packaged_task 的结果
	int data = res.get();
	std::cout << "run_in_main(): get the res = " << data << "\n";
}

int main(){
	std::cout << "main() start, thread id = " << std::this_thread::get_id() << "\n";
	
	run_in_main();
	run_in_thread();

	return 0;
}