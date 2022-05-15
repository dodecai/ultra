#pragma once

#include "Omnia/Core.h"

namespace Omnia {

/// @brief This class holds all available threads
class ThreadPool {
public:
    // Default
    ThreadPool(uint32_t threadCount = std::thread::hardware_concurrency()) {
        mWorkers.reserve(threadCount);
        for (std::size_t i = 0; i < threadCount; ++i) {
            mWorkers.emplace_back([this] {
                while (true) {
                    std::function<void()> task;
                    {
                        std::unique_lock<std::mutex> lock(mQueueMutex);
                        mCondition.wait(lock, [this] { return mStop || !mTasks.empty(); });
                        if (mStop && mTasks.empty()) return;
                        task = std::move(mTasks.front());
                        mTasks.pop();
                    }
                    task();
                }
            });
        }
    }
    ~ThreadPool() {
        std::unique_lock<std::mutex> lock(mQueueMutex);
        mStop = true;
        mCondition.notify_all();
        for (auto &worker : mWorkers) worker.join();
    }
    template <typename F, typename ... Args>
    auto Enqueue(F &&f, Args &&... args) {
        using return_type = typename std::invoke_result_t<F, Args ...>;

        auto task = std::make_shared<std::packaged_task<return_type()>>(std::bind(std::forward<F>(f), std::forward<Args>(args)...));
        auto result = task->get_future();
        {
            std::unique_lock<std::mutex> lock(mQueueMutex);
            if (mStop) throw std::runtime_error("Enqueue called on a stopped ThreadPool");
            mTasks.emplace([task]() { (*task)(); });
        }
        mCondition.notify_one();
        return result;
    }
    void Wait() {
        std::unique_lock<std::mutex> lock(mQueueMutex);
        mCondition.wait(lock, [this]() {
            return mTasks.empty();
        });
    }

    // Accessors
    const std::vector<std::thread> &GetWorkers() const { return mWorkers; }

private:
    // Properties
    std::queue<std::function<void()>> mTasks;
    std::vector<std::thread> mWorkers;

    std::condition_variable mCondition;
    std::mutex mQueueMutex;
    bool mStop = false;
};

}
