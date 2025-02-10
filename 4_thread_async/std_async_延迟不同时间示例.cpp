#include <iostream>
#include <thread>
#include <chrono>
#include <future>

int foo(int a, int b){ 
    std::cout << "foo(): thread id = " << std::this_thread::get_id() << "\n";
    std::cout << "foo(): starts caculating the res..." << "\n";
    std::this_thread::sleep_for(std::chrono::seconds(2)); // foo() 需要执行大概 2 秒
    std::cout << "foo(): ends caculating the res..." << "\n";
    return a + b;
}

int main(){
    std::cout << "main(): thread id = " << std::this_thread::get_id() << "\n";

	// (1) 创建一个异步任务
	// (1.1) std::launch::async 会立即执行 foo()，wait_for() 可能超时，可能成功获取结果
    std::future<int> res = std::async(std::launch::async, foo, 3, 4);
    // (1.2) std::launch::deferred 会延迟执行 foo()，直到某个调用 get() 或 wait()，wait_for() 不起作用
    // std::future<int> res = std::async(std::launch::deferred, foo, 3, 4);
	// (1.3) 默认情况（std::launch::async | std::launch::deferred），则不确定为 (1.1) 还是 (1.2)
	// std::future<int> res = std::async(foo, 3, 4);

	auto get_data = [&](std::future<int>& res){
		auto data = res.get();
		std::cout << "finally get the res = " << data << "\n";
	};

	// (2) 枚举类型，判断 res 的状态
	// std::future_status status = res.wait_for(std::chrono::seconds(1)); // 等待 1 秒，超时
	std::future_status status = res.wait_for(std::chrono::seconds(3)); // 等待 3 秒，成功获取结果
	if(status == std::future_status::timeout){
		// (2.1) foo() 还未执行完成，超过等待时间，没有返回结果，超时
		std::cout << "WARNING: time out" << "\n";
	}else if(status == std::future_status::ready){
		// (2.2) foo() 在等待时间内执行完毕，可以获取结果（参数为 std::launch::async）
		std::cout << "successfully get result." << "\n";
		get_data(res);
	}else if(status == std::future_status::deferred){
		// (2.3) 参数为 std::launch::deferred 就会出现此情况 
		std::cout << "thread is deferred." << "\n";
		get_data(res);
	}

	return 0;
}