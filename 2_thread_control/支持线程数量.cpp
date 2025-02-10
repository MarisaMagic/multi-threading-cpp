#include <iostream>
#include <thread>


int main(){
    size_t n = std::thread::hardware_concurrency();

    if(n == 0){
        std::cout << "The number of threads cannot be determined." << "\n";
    }else{
        std::cout << "The system supports approximately " << n << " threads." << "\n";
    }

    return 0;
}