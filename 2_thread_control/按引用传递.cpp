#include <iostream>
#include <thread>

// 线程接口函数
void foo(int& a, std::string& s){ 
    a ++ ;
    s += "Magic";
    std::cout << "foo(): " << a << " " << s << "\n";
}

int main(){
    int num = 520;
    std::string str = "Marisa";

    // 按引用传递参数
    std::thread t(foo, std::ref(num), std::ref(str)); 
    t.join();

    // 成功修改了原始数据
    std::cout << "main(): " << num << ' ' << str << "\n"; 

    return 0;
}