export module Ultra.Core.ThreadPool;

import Ultra.Core;

export namespace Ultra {

class ThreadPool {
public:
    ThreadPool(size_t threads = thread::hardware_concurrency()): mStop(false) {
        for (size_t i = 0; i < threads; i++) {
            mWorkers.emplace_back(
                [this] {
                    for(;;) {
                        function<void()> task;
                        {
                            std::unique_lock<mutex> lock(this->mQueueMutex);
                            this->mCondition.wait(lock, [this] {
                                return this->mStop || !this->mTasks.empty();
                            });
                            if (this->mStop && this->mTasks.empty()) return;
                            task = std::move(this->mTasks.front());
                            this->mTasks.pop();

                        }
                        task();
                    }
                }
            );
        }
    }
    ~ThreadPool() {
        {
            std::unique_lock<mutex>lock(mQueueMutex);
            mStop = true;
        }
        mCondition.notify_all();
        for (auto &worker : mWorkers) {
            worker.join();
        }
    }

    template<typename F, typename ...Args>
    auto Enqueue(F &&f, Args &&...args) -> future<typename std::invoke_result<F, Args...>::type> {
        using return_type = typename std::invoke_result<F, Args...>::type;

        auto task = std::make_shared<std::packaged_task<return_type()>>(
            std::bind(std::forward<F>(f), std::forward<Args>(args)...)
        );

        future<return_type> resource = task->get_future();
        {
            std::unique_lock<mutex> lock(mQueueMutex);

            if (mStop) throw std::runtime_error("Enqueue on stopped thread pool!");

            mTasks.emplace([task]() { (*task)(); });
        }

        mCondition.notify_one();
        return resource;
    }

private:
    // Worker-Threads, Tasks-Queue
    queue<function<void()>> mTasks;
    vector<thread> mWorkers;

    // Synchronization
    bool mStop;
    condition_variable mCondition;
    mutex mQueueMutex;
};

}
