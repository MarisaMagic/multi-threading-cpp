#include <iostream>
#include <thread>
#include <vector>
#include <string>

/* 只读的数据，是安全稳定的，不需要做特殊的处理 */

std::vector<std::string> data{"Marisa", "Alice", "Reimu"};

void func(int num){
	std::cout << "thread id: " << std::this_thread::get_id() << "\n";
	std::cout << "shared data: ";
	for(const auto &x : data) std::cout << x << " ";
	std::cout << "\n";	
}

int main(){
	std::vector<std::thread> vt;
	for(int i = 0; i < 10; i ++ ){
		vt.emplace_back(std::thread(func, i));
	}

	// for(auto &t : vt) t.join();

	for(auto it = begin(vt); it != end(vt); it ++ ) it->join(); // (*it).join();

	return 0;
}