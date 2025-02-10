#include <iostream>
#include <thread>
#include <future>
#include <chrono>

int add(int a, int b){ return a + b; }

void foo(std::shared_future<int>& shared_res, 
		std::chrono::time_point<std::chrono::high_resolution_clock>& start){
	auto data = shared_res.get();
    // 输出获取结果的时间
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> dura = end - start;
	std::cout << "foo(): duration = " << dura.count() << "ms" << "\n";
}

int main(){
	std::future<int> res = std::async(std::launch::async, add, 1, 2);
	std::shared_future<int> shared_res = res.share(); // 内部 std::move

	// 获取起始时间点
	auto start = std::chrono::high_resolution_clock::now();

	// (1) 开启一个子线程 t1 等待获取共享结果
	std::thread t1(foo, std::ref(shared_res), std::ref(start)); // 调用引用
	
	// (2) 开启另一个子线程 t2 等待获取共享结果
	std::thread t2([&](){
		auto data = shared_res.get();
        // 输出获取结果的时间
		auto end = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double, std::milli> dura = end - start;
		std::cout << "lambda(): duration = " << dura.count() << "ms" << "\n";
	});

	// (3) 主线程中等待获取共享结果
	auto data = shared_res.get();
    // 输出获取结果的时间
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> dura = end - start;
    std::cout << "main(): duration = " << dura.count() << "ms" << "\n";

	t1.join();
	t2.join();

	return 0;
}