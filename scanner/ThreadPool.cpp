#include "ThreadPool.h"
#include <iostream>

ThreadPool::ThreadPool(size_t numThreads) {
    for(size_t i = 0; i < numThreads; ++i) {
        workers.emplace_back([this] {
            while(true) {
                std::function<void()> task;
                {
                    std::unique_lock<std::mutex> lock(this->queueMutex);
                    this->condition.wait(lock, [this] { 
                        return (this->stop || !this->tasks.empty()); 
                    });
                    
                    if(this->stop && this->tasks.empty()) {
                        return;
                    }
                    
                    task = std::move(this->tasks.front());
                    this->tasks.pop();
                }
                task();
            }
        });
    }
}

void ThreadPool::enqueue(std::function<void()> task) {
    {
        std::unique_lock<std::mutex> lock(queueMutex);
        if(stop) {
            throw std::runtime_error("enqueue on stopped ThreadPool");
        }
        tasks.push(std::move(task));
    }
    condition.notify_one();
}

void ThreadPool::waitAll() {
    while(true) {
        std::unique_lock<std::mutex> lock(queueMutex);
        if(tasks.empty()) {
            break;
        }
        lock.unlock();
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

ThreadPool::~ThreadPool() {
    {
        std::unique_lock<std::mutex> lock(queueMutex);
        stop = true;
    }
    condition.notify_all();
    for(std::thread &worker : workers) {
        worker.join();
    }
}