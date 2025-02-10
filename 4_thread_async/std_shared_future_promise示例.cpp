#include <iostream>
#include <thread>
#include <future>

void foo_set_value(std::promise<int>& pro, int x){
	std::cout << "foo_set_value(): starts running, thread id = ";
    std::cout << std::this_thread::get_id() << "\n";

    // 模拟数据操作需要的时间
    std::this_thread::sleep_for(std::chrono::milliseconds(2000)); 
	
    // 中途 set_value 来传递操作后的数据给其他线程
	pro.set_value(x); 
	
	// 线程函数模拟进行其他操作
    std::this_thread::sleep_for(std::chrono::milliseconds(2000)); 
	
	std::cout << "foo_set_value(): ends running, thread id = ";
    std::cout << std::this_thread::get_id() << "\n";
}

void foo_get_res(std::shared_future<int>& shared_res){
    std::cout << "foo_get_res(): starts running, thread id = ";
    std::cout << std::this_thread::get_id() << "\n";

    // 等待获取结果
    int data = shared_res.get();
    std::cout << "foo_get_res(): get the res = " << data << "\n";

    std::cout << "foo_get_res(): ends running, thread id = ";
    std::cout << std::this_thread::get_id() << "\n";
}   

int main() {
	std::cout << "main(): starts running, thread id = ";
    std::cout << std::this_thread::get_id() << "\n";
	
	// 通过 std::promise 实现子线程和主线程之间的线程通信
	std::promise<int> pro; // 声明一个 promise 对象，保存值类型为 int
	std::future<int> res = pro.get_future(); // 创建 std::future 获取数据
    std::shared_future<int> shared_res = res.share(); // 共享 future 对象 
	std::thread t_set_value(foo_set_value, std::ref(pro), 1314); // 创建子线程
	
	// 主线程调用 get() 等待获取结果。若子线程内部还未 set_value，会阻塞当前主线程 
	int data = shared_res.get();
	std::cout << "main(): get the res = " << data << "\n";

    // 开启一个新的子线程，也等待获取结果
    std::thread t_get_res(foo_get_res, std::ref(shared_res));

    // 主线程模拟进行其他操作
    std::cout << "main(): starts doing something else..." << "\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(3000)); 
    std::cout << "main(): ends doing something else..." << "\n";

	// join()，等待子线程的其他操作执行完毕
    t_get_res.join();
	t_set_value.join();

	std::cout << "main(): ends running, thread id = ";
    std::cout << std::this_thread::get_id() << "\n";

    return 0;
}