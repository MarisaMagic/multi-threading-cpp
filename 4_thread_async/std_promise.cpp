#include <iostream>
#include <thread>
#include <chrono>
#include <future>

void foo(std::promise<int> &pro, int x){
	std::cout << "foo(): starts running, thread id = ";
    std::cout << std::this_thread::get_id() << "\n";
	
	// 模拟数据操作所需时间
	std::this_thread::sleep_for(std::chrono::milliseconds(2000));
	x ++ ;

    // 中途 set_value 来传递操作后的数据给其他线程
	pro.set_value(x); 
	
	// 模拟线程函数其他操作
	std::this_thread::sleep_for(std::chrono::milliseconds(2000)); 
	
	std::cout << "foo(): ends running, thread id = ";
    std::cout << std::this_thread::get_id() << "\n";
}

int main(){
	std::cout << "main(): starts running, thread id = ";
    std::cout << std::this_thread::get_id() << "\n";
	
	// 通过 std::promise 实现子线程和主线程之间的线程通信
	std::promise<int> pro; // 声明一个 promise 对象，保存值类型为 int
	std::future<int> res = pro.get_future(); // 创建 std::future 获取数据 
	std::thread t(foo, std::ref(pro), 1); // 创建子线程
	
	// 主线程调用 get() 获取结果。若子线程内部还未 set_value，会阻塞当前主线程 
	int data = res.get();
	std::cout << "finally get the res = " << data << "\n";
	
	// 获取结果才 join()，等待子线程的其他操作执行完毕
	t.join();

	std::cout << "main(): ends running, thread id = ";
    std::cout << std::this_thread::get_id() << "\n";
	
	return 0;
}