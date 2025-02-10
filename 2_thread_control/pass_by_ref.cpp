#include <iostream>
#include <thread>
#include <string>

void update_data(int &data1, const int &data2, const std::string &s){
	data1 ++ ;
	std::cout << "ref data1 = " << data1 << "\n";
	std::cout << "cref data2 = " << data2 << "\n";
	std::cout << s << "\n";
}

void build_thread(){
	int a = 1, b = 3;
	char s[] = "Kirisame Marisa";

	// thread 构造函数只是盲目拷贝提供的值（无法进行真正传递引用）
	// 内部代码会将拷贝参数作为右值传递，此时会编译失败
	// 因此必须外层用ref来进行引用传递。
	std::thread t(update_data, std::ref(a), std::cref(b), std::string(s));
	t.join();
	std::cout << "a = " << a << "\n";
	std::cout << "b = " << b << "\n";
}

/*
 在使用detach时
 若传递int这种简单变量，建议传值;
 如果传递类对象，避免隐式类型转换。调用时就构造临时对象，在函数形参中用引用来接
*/

int main(){
	build_thread();

	return 0;
}