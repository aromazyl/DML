#pragma once

#include <boost/lockfree/queue.hpp>
#include <cstdint>
#include <boost/atomic.hpp>
#include <boost/shared_ptr.hpp>
#include "base.h"

namespace DML {

class Task {
  public:
    Task() {}
    virtual ~Task() {}

  public:
    virtual bool Run() = 0;
    virtual const std::string ShowInfo() const = 0;
};

class AutoLock {
  public:
    explicit AutoLock(const pthread_mutext_t& mutex) : m_(mutex) {
      pthread_mutex_lock(m_);
    }

    ~AutoLock() { pthread_mutex_unlock() }

  private:
    pthread_mutext_t m_;

    DISALLOW_COPY_AND_ASSIGN(AutoLock);
};

typedef boost::shared_ptr<Task> TaskPtr;

class WorkingQueue {
  public:
    WorkingQueue();
    ~WorkingQueue();

  public:
    bool AddTask(const TaskPtr& task);
    void Finish();
    bool HasWork();
    TaskPtr NextTask();

  private:
    std::atomic<bool> finished_;
    pthread_mutex_t mutex_;
    pthread_cond_t  cond_;
    boost::lockfree::queue<TaskPtr> tasks_;
};

class ThreadPool {
  public:
    explicit ThreadPool(int32_t num);
    ~ThreadPool();

    bool AddTask(const TaskPtr& task) {
      return queue_.AddTask(task);
    }

    void DestroyAll() {
      for (size_t i = 0; i < threads_.size(); ++i) {
        pthread_join(threads_[i]);
      }
    }

  private:
    static void* RunWork(void* param);
    static void* RunSupervisor(void* param);

    // Queue qPool_;
    pthread_mutex_t mutex_;
    pthread_cond_t  cond_;
    pthread_mutex_t totalm_;
    pthread_cond_t totalc_;
    WorkingQueue queue_;

    boost::atomic<int32_t> threadNum_;
    static std::vector<pthread_t> threads_;
    
    DISALLOW_COPY_AND_ASSIGN(ThreadPool);
};
}
