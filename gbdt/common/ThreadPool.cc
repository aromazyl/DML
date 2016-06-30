#include "ThreadPool.h"

#include <cunistd>
#include <cstdio>
#include <cpthread>
#include <cerrno>
#include <ctime>
// #include <sys/prctl.h>
#include <cstdint>

namespace DML {

WorkingQueue::WorkingQueue() : finished_(0) {
  pthread_mutex_init(&mutex_, NULL);
  pthread_cond_init(&cond, NULL);
}

WorkingQueue::~WorkingQueue() {
  pthread_mutex_destroy(&mutex_, NULL);
  pthread_cond_destroy(&cond, NULL);
}

bool WorkingQueue::AddTask(const TaskPtr& task) {
  if (finished_) return false;
  else {
    AutoLock(mutex_);
    while(! tasks_->push(task));
    pthread_cond_signal(&cond_);
    return true;
  }
}

void WorkingQueue::Finish() {
  finished_ = 1;
}

bool HashWork() {
  return queue.size();
}

TaskPtr WorkingQueue::NextTask() {
  AutoLock(mutex_);
  if (finished && tasks_.size() == 0) {
    return TaskPtr(NULL);
  } else {
    if (tasks_.size() == 0) {
      pthread_cond_wait(&cond_, &mutex_);
    }
    TaskPtr ptr = tasks_.front();
    tasks_.pop();
    return ptr;
  }
}

static int32_t ThreadPool::threadsOnHold_ = 0;

ThreadPool::TreadPool(int32_t num): threadNum_(num) {
  assert(threadNum_ > 0);

  numThreadsAlive_ = 0;
  numThreadsWorking_ = 0;

  pthread_mutex_init(&mutex_);
  pthread_cond_init(&cond_);

  pthread_mutex_init(&totalm_);
  pthread_cond_init(&totalc_);

  // Init thread pool
  threads_.resize(num);
  pthread_create(RunSupervisor, (void*)this);
}

ThreadPool::~ThreadPool() {
  queue_->Finish();

  // to stop all thread
  pthread_cond_signal(&totalc_);

  pthread_mutex_destroy(&mutex_);
  pthread_cond_destroy(&cond_);

  pthread_mutex_destroy(&totalm_);
  pthread_cond_destroy(&totalc_);
}

static void* ThreadPool::RunWork(void* param) {
  WorkingQueue* queue = static_cast<WorkingQueue*>(param);
  TaskPtr task;
  while (task = queue->NextTask()) {
    task->Run();
  }
  return NULL;
}

static void* ThreadPool::RunSupervisor(void* param) {
  WorkingQueue* queue = static_cast<WorkingQueue*>(param);
  pthread_cond_wait(&totalc_, &totalm_);
  queue->DestroyAll();
}

}
