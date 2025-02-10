#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>
#include <future>

int foo(std::string s, std::string t){
	std::cout << s << " " << t << "\n";
	std::cout << "foo() start, thread id = " << std::this_thread::get_id() << "\n";
	std::this_thread::sleep_for(std::chrono::milliseconds(3000));
	std::cout << "foo() end, thread id = " << std::this_thread::get_id() << "\n";
	return 520;
}

void foo2(std::shared_future<int> &res){
	std::cout << "foo2() start, thread id = " << std::this_thread::get_id() << "\n";
	auto data = res.get();
	std::cout << "foo2() res = " << data << "\n";
	std::this_thread::sleep_for(std::chrono::milliseconds(3000));
	std::cout << "foo2() end, thread id = " << std::this_thread::get_id() << "\n";
}

int main(){
	// std::packaged_task: 打包任务，将任务进行包装
	std::cout << "main() start, thread id = " << std::this_thread::get_id() << "\n";
	
	// 包装
	std::packaged_task<int(std::string, std::string)> pt(foo);
	std::thread t1(std::ref(pt), "Marisa", "Alice"); 
	t1.join();

	// 直接创建shared_future
	std::shared_future<int> shared_res = pt.get_future();

	// 获取结果
	auto data = shared_res.get();
	data = shared_res.get();
	std::cout << "main() res = " << data << "\n";

	std::thread t2(foo2, std::ref(shared_res)); // 需要传右值
	t2.join();

	std::cout << "main() end, thread id = " << std::this_thread::get_id() << "\n";

	return 0;
}