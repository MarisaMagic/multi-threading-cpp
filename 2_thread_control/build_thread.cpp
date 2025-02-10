#include <iostream>
#include <thread>

/*
 主线程从main开始执行，主线程从主函数return，则整个程序结束
 如果其他子线程还未执行完（没有join()或detach()），系统会强行终止
 创建的子线程从一个函数开始执行，函数执行完毕，线程运行结束 
*/

// 1. 用函数创建线程
void func1(int num){
	for(int i = 1; i < num; i ++ ){
		std::cout << "Kirisame Marisa " << i << "\n";
	}
}

void build_thread_1(){
	std::thread t(func1, 10);
	t.join(); // join() 等待子线程执行完毕，然后子线程和主线程汇合，再往下执行
	// t.detach(); // detach() 分离子线程和主线程，不等待子线程，若主线程结束，子线程就会驻留系统在后台运行

	/*
	 每个线程只能join一次，一旦线程进行join()，也不能再detach()
	 一旦把线程detach()，也不能再join()
	*/
}


// 2. 用类（仿函数）创建线程
class node{
	int num;
public:
	node(int n): num(n){}
	void operator()(int arg){
		for(int i = 1; i < num; i ++ ){
			std::cout << "Alice Margatroid " << i << "\n";
		}
	}
};

void build_thread_2(){
	node a(10);
	std::thread t(a, 3);
	t.join();

	/*
	 假设这个类中引用了主线程的变量，而且子线程创建后调用detach() 了，
	 如果主线程先执行完，这个被引用的变量就会被回收，而此时子线程（没执行完）仍在引用这块内存空间，会导致不可预料的结果
	 thread t(a, 3); 这样构造子线程，子线程是基于 a 的对象副本的，放心用 detach
	 thread t(std::ref(a), 3); 这样构造子线程，子线程是基于 a 自身的，用 detach 可能出错，应采用 join
	*/
}


// 3. 用类成员函数指针创建线程
class A{
	int val, val2;
public:
	A(int n, int m): val(n), val2(m){
		std::cout << "build function thread id: " << std::this_thread::get_id() << "\n";
	}
	A(const A &a){
		val = a.val;
		val2 = a.val2;
		std::cout << "copy build function thread id: " << std::this_thread::get_id() << "\n";
	}
	A(A&& a){
		val = a.val;
		val2 = a.val2;
		std::cout << "move function thread id: " << std::this_thread::get_id() << "\n";
	}
	~A(){
		std::cout << "~function thread id: " << std::this_thread::get_id() << "\n";
	}

	void foo(){ 
		std::cout << "foo(): thread starts, thread id = " << std::this_thread::get_id() << "\n";
		// 模拟做一些事情
		std::cout << "foo(): MarisaMagic find the val is " << val << ' ' << val2 << "\n";
		std::this_thread::sleep_for(std::chrono::seconds(1)); 
		std::cout << "foo(): thread ends, thread id = " << std::this_thread::get_id() << "\n";
	}
};

void build_thread_3(){
	A a(10, 11);

	// 拷贝
	std::thread t1(&A::foo, a); //类成员函数指针，类对象，子线程参数（基于对象的副本，因此会调用拷贝构造函数）
	t1.join();
	// t.detach();  // 可以使用detach

	// std::thread t3(&A::foo, &a);
	// t3.join();

	// 引用
	// std::thread t2(&A::foo, std::ref(a)); // 子线程是基于 a 对象自身了，此时用detach可能会出问题
	// t2.join();  // 建议采用join

	// 移动
	// std::thread t(&A::foo, std::move(a)); // 需要显式声明移动构造函数
	// t.join();  
}


// 4. 用lambda表达式创建线程
void build_thread_4(){
	// (1)
	auto func = [](int a, int b){
		std::cout << "Marisa " << a << "\n";
		std::cout << "Alice " << b << "\n";
	};

	std::thread t(func, 1, 2);
	t.join();

	// (2)
	std::thread t2 = std::thread([](int a, int b){
		std::cout << "Marisa " << a << "\n";
		std::cout << "Alice " << b << "\n";
	}, 3, 4);
	t2.join();
}

int main(){
	// build_thread_1();

	// build_thread_2();

	build_thread_3();

	// build_thread_4();

	return 0;
}