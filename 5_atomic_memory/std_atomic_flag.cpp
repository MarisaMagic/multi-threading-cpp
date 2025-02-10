#include <iostream>
#include <thread>
#include <atomic>
#include <vector>

class spinlock_mutex{
    std::atomic_flag flag;
public:
    spinlock_mutex(): flag(ATOMIC_FLAG_INIT) {};
    void lock(){
        while(flag.test_and_set());
    }
    void unlock(){
        flag.clear();
    }
};

int count;
spinlock_mutex mtx;

void foo(){
    mtx.lock();
    count ++ ;
    mtx.unlock();
}

int main(){
    std::vector<std::thread> threads(10);
    for(auto& t : threads) t = std::thread(foo);
    for(auto& t : threads) t.join();

    std::cout << "count = " << count << "\n";

    return 0;
}