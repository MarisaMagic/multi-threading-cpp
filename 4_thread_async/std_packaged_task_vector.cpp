#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>
#include <vector>
#include <future>

int foo(std::string s, std::string t){
	std::cout << s << " " << t << "\n";
	std::cout << "foo() start, thread id = " << std::this_thread::get_id() << "\n";
	std::this_thread::sleep_for(std::chrono::milliseconds(3000));
	std::cout << "foo() end, thread id = " << std::this_thread::get_id() << "\n";
	return 520;
}

int main(){
	// std::packaged_task: 打包任务，将任务进行包装
	// 是一个类模板，模板参数为各种可调用对象
	// 通过packaged_task 将各种可调用参数对象包装，方便作为线程入口函数
	std::cout << "main() start, thread id = " << std::this_thread::get_id() << "\n";
	
	// 包装
	std::packaged_task<int(std::string, std::string)> pt(foo);

	std::vector<std::packaged_task<int(std::string, std::string)> > pt_v;
	pt_v.push_back(std::move(pt));  // 采用了移动语义，入容器后，pt变为空

	// 获取future
	std::future<int> res = pt_v[0].get_future();

	// 在后期定义线程时，可以调用对象包装引用
	std::thread t1(std::ref(pt_v[0]), "Marisa", "Alice"); 
	t1.join();

	// 获取packaged_task 结果
	int data = res.get();
	std::cout << data << "\n";

	return 0;
}