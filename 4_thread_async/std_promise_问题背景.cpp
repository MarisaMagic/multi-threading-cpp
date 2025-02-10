#include <iostream>
#include <thread>
#include <chrono>
#include <future>

void foo(int& res, int x){
	std::cout << "foo(): starts running, thread id = ";
    std::cout << std::this_thread::get_id() << "\n";
	
	// 模拟数据操作所需时间
	std::this_thread::sleep_for(std::chrono::milliseconds(2000));
	x ++ ;

    // 将 res 置为 x，子线程执行完毕后再用于其他线程
	res = x; 
	
	// 模拟线程函数其他操作
	std::this_thread::sleep_for(std::chrono::milliseconds(2000)); 
	
	std::cout << "foo(): ends running, thread id = ";
    std::cout << std::this_thread::get_id() << "\n";
}

int main(){
	std::cout << "main(): starts running, thread id = ";
    std::cout << std::this_thread::get_id() << "\n";
	
	// 通过 std::promise 实现子线程和主线程之间的线程通信
    int res = 0;
	std::thread t(foo, std::ref(res), 1); // 创建子线程
	
	// 子线程数据操作需要一段时间，不知道什么时候可以获取正确的数据结果
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	std::cout << "may get the wrong res = " << res << "\n";
	
	// 调用 join()，等待子线程的其他操作执行完毕
	t.join();

    // 等待子线程执行完毕，可以顺利获取正确的数据结果
    std::cout << "finally get the res = " << res << "\n";

	std::cout << "main(): ends running, thread id = ";
    std::cout << std::this_thread::get_id() << "\n";
	
	return 0;
}