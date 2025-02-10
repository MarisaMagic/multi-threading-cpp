#include <iostream>
#include <thread>
#include <mutex>
#include <future>
#include <condition_variable>
#include <functional>
#include <vector>
#include <queue>

class ThreadPool{
    void work(); // 线程执行的函数（任务内容）
    bool stop; // 线程池是否停止标记
    std::queue<std::function<void()>> tasks; // 任务队列，任务被包装为 void function
    std::vector<std::thread> threads; // 线程池
    std::condition_variable cv; // 条件变量，用于唤醒等待的线程
    std::mutex mtx; // 互斥量，用于互斥访问标记、任务队列
public:
    // 构造函数，传入线程池中线程数量
    ThreadPool(int thread_num); 

    // 析构函数
    ~ThreadPool();

    // 添加任务到任务队列函数，返回类型后置（参数：函数，可变参数模板）
    // 传入万能引用，返回一个 std::future 对象（获取结果为 std::result_of<F(Arg...)>::type）
    template<typename F, typename... Arg>
    auto submit(F&& f, Arg&&... args) -> std::future<typename std::result_of<F(Arg...)>::type>;
};

// 构造函数
ThreadPool::ThreadPool(int thread_num): stop(false) {
    for(size_t i = 0; i < thread_num; i ++ ){
        threads.emplace_back(&ThreadPool::work, this);
    }
}

// 析构函数
ThreadPool::~ThreadPool(){
    {
        std::lock_guard<std::mutex> lck(mtx); // 加锁，互斥访问 stop 标记
        stop = true;
    }

    // 通知所有等待中的线程，线程池已经停止
    cv.notify_all();

    // 等待所有子线程运行完毕
    for(auto& t : threads){
        if(t.joinable()){
            t.join();
        }
    }
}

// 提交任务函数
template<typename F, typename... Arg>
auto ThreadPool::submit(F&& f, Arg&&... args) -> std::future<typename std::result_of<F(Arg...)>::type>{
    // 执行 f 函数返回的数据类型
    using func_type = typename std::result_of<F(Arg...)>::type;

    // 通过智能指针，指向函数模板为 func_type() 的包装任务，避免共同访问时被销毁
    // 通过 std::bind 绑定函数、可变参数列表得到一个包装任务
    // std::forward 用于完美转发，将参数以原始的类型（左值或右值）传递
    auto task = std::make_shared<std::packaged_task<func_type()>>(
        std::bind(std::forward<F>(f), std::forward<Arg>(args)...)
    );

    // 添加任务到任务队列
    {
        std::lock_guard<std::mutex> lck(mtx);
        if(stop){
            throw std::runtime_error("ERROR: The thread pool is stoped.");
        }
        // 将前面构造好的可调用对象 packaged_task 添加任务队列
        tasks.emplace([task](){ // 捕获 task 智能指针
            (*task)(); // 解引用，获取指向的包装任务，并通过()调用这个任务
        });
    }

    // 唤醒一个等待的线程来执行任务
    cv.notify_one();

    // 返回 std::future 对象，后续等待获取结果
    return task -> get_future();
}

// 线程执行的函数
void ThreadPool::work(){
    while(true){
        // 定义一个任务
        std::function<void()> task;

        // 从任务队列中取出一个任务
        {
            std::unique_lock<std::mutex> lck(mtx);
            while(tasks.empty() && !stop){ // 避免虚假唤醒
                cv.wait(lck); // 需要等待任务进队，线程陷入阻塞
            }
            // 可能由于 stop 而退出上面的循环，此时若任务队列为空，直接返回
            if(tasks.empty() && stop){ // 任务队列为空且线程池停止
                return;
            }
            task = std::move(tasks.front());
            tasks.pop();
        }

        // 执行任务
        task();
    }
}