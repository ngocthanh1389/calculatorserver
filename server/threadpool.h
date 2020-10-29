#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <sys/socket.h>
#include <thread>
#include <mutex>
#include <list>
#include <algorithm>
#include <condition_variable>
#include <unordered_set>

#include "calculator.h"

using namespace std;

class thread_pool
{
    list<int> work_list; // ports list to do
    unordered_set<int> actives; // active ports set
    vector<thread> threads;
    mutex work_mutex;
    condition_variable work_wait;

    void worker_thread() {
        while(true)
        {
            unique_lock<mutex> guard(work_mutex);
            work_wait.wait(guard,[&]{return !work_list.empty();});
            int task = work_list.front();
            work_list.pop_front();
            actives.insert(task);
            work_mutex.unlock();
            auto rs = cal(task);
            work_mutex.lock();
            actives.erase(task);
            work_mutex.unlock();
            send(task, rs.c_str(), rs.length(), 0);
        }
    }

public:
    thread_pool() {
        auto thread_count = std::thread::hardware_concurrency();
        printf("Number core: %d\n", thread_count);
        for(unsigned i=0; i<thread_count; ++i) {
            threads.push_back(std::thread(&thread_pool::worker_thread, this));
            threads[i].detach();
        }
    }

    void submit(int task){
        lock_guard<mutex> guard(work_mutex);
        if (find(work_list.begin(), work_list.end(), task) == work_list.end() && actives.count(task) == 0) {
            work_list.push_back(task);
            work_wait.notify_all();
        }
    }
};
#endif // THREADPOOL_H
