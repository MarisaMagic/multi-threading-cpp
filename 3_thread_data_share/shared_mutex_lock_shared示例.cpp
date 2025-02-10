#include <iostream>
#include <thread>
#include <shared_mutex>
#include <chrono>
#include <vector>

std::shared_mutex s_mtx;
int count;

void read(int id){
    for(int i = 0; i < 5; i ++ ){
        if(id & 1){
            s_mtx.lock_shared(); // shared_mutex 作为共享锁使用
            std::cout << "read thread id = " << id;
            std::cout << ", count = " << count << "\n";
            s_mtx.unlock_shared();
        }else{
            if(s_mtx.try_lock_shared()){
                std::cout << "read thread id = " << id;
                std::cout << ", count = " << count << "\n";
                s_mtx.unlock_shared();
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

void write(){
    s_mtx.lock(); // shared_mutex 作为独占锁使用
    count ++ ;
    std::cout << "write thread id = " << std::this_thread::get_id();
    std::cout << ", count = " << count << "\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    s_mtx.unlock();
}

int main(){
    std::vector<std::thread> readers(5);
    for(size_t i = 0; i < readers.size(); i ++ ){
        readers[i] = std::thread(read, i);
    }
    std::thread writer(write);

    for(auto& t : readers) t.join();
    writer.join();

    return 0;
}