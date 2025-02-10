#include <iostream>
#include <thread>
#include <future>
#include <chrono>

int add(int a, int b){ return a + b; }

void foo(std::shared_future<int>& shared_res){
	std::cout << "foo(): thread id = " << std::this_thread::get_id() << "\n";
	std::cout << "foo(): get the shared res = " << shared_res.get() << "\n";
}

int main(){
	std::cout << "main(): thread id = " << std::this_thread::get_id() << "\n";
	
	// std::async 创建一个异步任务，并获取关联的 std::future 对象
	std::future<int> res = std::async(std::launch::async, add, 1, 2);
	// 通过 share 获取共享的异步任务 std::future 对象
	std::shared_future<int> shared_res = res.share(); // 内部 std::move

	// (1) 开启一个子线程 t1 等待获取共享结果
	std::thread t1(foo, std::ref(shared_res)); // 传入引用
	
	// (2) 开启另一个子线程 t2 等待获取共享结果
	std::this_thread::sleep_for(std::chrono::milliseconds(500)); // 休息一会，便于打印
	std::thread t2([&](){
		std::cout << "lambda(): thread id = " << std::this_thread::get_id() << "\n";
		auto data = shared_res.get();
		std::cout << "lambda(): get the shared res = " << data << "\n";
	});

	// (3) 主线程中等待获取共享结果
	std::this_thread::sleep_for(std::chrono::milliseconds(500)); // 休息一会，便于打印
	auto data = shared_res.get();
	std::cout << "main(): get the shared res = " << data << "\n";

	t1.join();
	t2.join();

	return 0;
}