#pragma once

#include <memory>
#include <future>
#include <thread>
#include <functional>
#include <mutex>
#include <condition_variable>
#include <atomic>

#include "../vector.h"
#include "../queue.h"

namespace tinystl {
namespace net {


class ThreadPool
{
    enum ThreadInitData { N_THREADS = 10 };
public:
    ThreadPool(int threadNums = ThreadInitData::N_THREADS) noexcept
        : quit_(false)
    {
        while(threadNums--)
        {
            threads_.emplace_back(
                      [this]()
                      {
                        while(!this->quit_)
                        {
                            Task task;
                            {
                                std::unique_lock<std::mutex> ul(this->mutex_);
                                this->cond_.wait(ul, [this]() { return this->quit_ || !this->tasks_.empty(); }); 
                                if(this->quit_) return;
                                task = std::move(this->tasks_.front());
                                this->tasks_.pop();
                            }
                            task();
                        }
                      }
                    );
        }
    }
    ~ThreadPool()
    {
        {
            std::unique_lock<std::mutex> lock(mutex_);
            quit_ = true;
        }
        cond_.notify_all();
        for(auto& th : threads_)
          th.join();
    }

public:
    template <class F, class... Args>
    auto enqueue(F&& f, Args&&... args)
    -> std::future<typename std::result_of<F(Args...)>::type>
    {
        using return_type = typename std::result_of<F(Args...)>::type;
        auto task = std::make_shared<std::packaged_task<return_type()>> (
                        std::bind(std::forward<F>(f), std::forward<Args>(args)...)
                    );
        std::future<return_type> res = task->get_future();
        {
            std::unique_lock<std::mutex> lock(mutex_);
            if(quit_)
                throw std::runtime_error("enqueue on quited threadpool");
            tasks_.push([task]() { (*task)(); });
        }
        cond_.notify_one();
        return res;
    }
    
private:
    typedef tinystl::vector<std::thread> ThreadList;
    typedef std::function<void()> Task;
    typedef tinystl::queue<Task> TaskList;

    ThreadList threads_;
    TaskList tasks_;
    std::mutex mutex_;
    std::condition_variable cond_;
    std::atomic<bool> quit_;

    
};





}
}
