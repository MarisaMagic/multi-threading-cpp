#include <iostream>
#include <thread>

int main(){
	// (1)
	std::thread t([](int a, int b){
		std::cout << "Marisa " << a << "\n";
		std::cout << "Alice " << b << "\n";
	}, 1314, 520);
	t.join();

    // (2)
	auto foo = [](int a, int b){
		std::cout << "Marisa " << a << "\n";
		std::cout << "Alice " << b << "\n";
	};

	std::thread t2(foo, 520, 1314);
	t2.join();

    return 0;
}