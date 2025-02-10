#include <iostream>
#include <thread>

class MyClass{
	int val;
public:
	MyClass(int n): val(n) {}
	void operator() (int a) {
		for(int i = 1; i <= val; i ++ ){
			std::cout << "Alice Margatroid " << a << "\n";
		}
	}
};

int main(){ 
	MyClass obj(5);
    std::thread t(obj, 520);
	t.join();

    return 0;
}