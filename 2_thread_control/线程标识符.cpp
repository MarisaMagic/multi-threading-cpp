#include <iostream>
#include <thread>
#include <vector>

void foo(){
    std::cout << "thread id = " << std::this_thread::get_id() << "\n";
}

int main(){
    std::vector<std::thread> pv(10);
    for(auto& t : pv) t = std::thread(foo);
    for(auto& t : pv){
        if(t.joinable()){
            t.join();
        }
    }

    return 0;
}