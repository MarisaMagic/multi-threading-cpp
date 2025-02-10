#include <iostream>
#include <semaphore>
#include <thread>
#include <chrono>
 
std::binary_semaphore sm1(0), sm2(0);
 
void foo(){
    sm1.acquire(); // 等待主进程的信号
 
    std::cout << "foo(): Got the signal\n"; 
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << "foo(): Send the signal\n"; 
 
    sm2.release(); // 向主进程发出返回信号
}
 
int main(){
    std::thread t(foo);
 
    std::cout << "main(): Send the signal\n";
    sm1.release(); // 向子线程发出信号可以开始工作
    sm2.acquire(); // 等待子线程完成工作
    std::cout << "main(): Got the signal\n"; 
    
    t.join();

    return 0;
}