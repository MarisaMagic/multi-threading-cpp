#include <iostream>
#include <thread>
#include <shared_mutex>
#include <chrono>
#include <vector>

class Reader_Writer{
    std::shared_mutex s_mtx;  // 互斥量
    int val = 0;              // 共享数据
public:
    // 读操作
    void read_data(int id){
        std::shared_lock<std::shared_mutex> lck(s_mtx); // 共享锁
        std::cout << "Reader " << id << " reads the value = " << val << "\n";
    }
    // 读者，进行 100 次读操作
    void reader(int id){
        for(int i = 0; i < 100; i ++ ){
            read_data(id);
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }
    // 写操作
    void write_data(int id){
        std::unique_lock<std::shared_mutex> lck(s_mtx); // 独占锁
        val ++ ;
        std::cout << "Writer " << id << " writes the value = " << val << "\n";
    }
    // 写者，进行 10 次写操作
    void writer(int id){
        for(int i = 0; i < 10; i ++ ){
            write_data(id);
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        }
    }
};

int main(){
    Reader_Writer obj;
    // 创建 10 个读者线程和 2 个写者线程
    std::vector<std::thread> readers(10);
    for(size_t i = 0; i < readers.size(); i ++ ){
        readers[i] = std::thread(&Reader_Writer::reader, &obj, i);
    }
    std::vector<std::thread> writers(2);
    for(size_t i = 0; i < writers.size(); i ++ ){
        writers[i] = std::thread(&Reader_Writer::writer, &obj, i);
    }

    for(auto& reader : readers) reader.join();
    for(auto& writer : writers) writer.join();

    return 0;
}