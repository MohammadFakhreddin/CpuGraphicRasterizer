#include "./ThreadPool.h"

#include "../log/Logger.h"

ThreadPool::ThreadPool()
  :
  isThreadPoolAlive(true)
{

  numberOfThreads = (unsigned int)(float(std::thread::hardware_concurrency()) / 2.0f);
  //numberOfThreads = 1;
  if (numberOfThreads == 0) {
    numberOfThreads = 1;
  }

  Logger::log("Initiating thread pool with " + std::to_string(numberOfThreads) + " threads.");

  if (numberOfThreads < 2) {

    Logger::log("This system cannot use threads");
    isThreadPoolAlive = false;
  
  } else {

    threadObjects.reserve(numberOfThreads);
    
    for (unsigned int threadIndex = 0; threadIndex < numberOfThreads; threadIndex++) {
      threadObjects.emplace_back(std::make_unique<ThreadObject>(threadIndex));
    }

  }

}

ThreadPool::~ThreadPool() {

  isThreadPoolAlive = false;

  if (numberOfThreads > 1) {

    for (unsigned int threadIndex = 0; threadIndex < numberOfThreads; threadIndex++) {
      threadObjects[threadIndex]->notifyThreadPoolIsDead();
    }

    for (auto& thread : threadObjects) {
      thread->join();
    }

  }
}

void ThreadPool::assignTask(
  const unsigned int& threadNumber,
  std::function<void(const unsigned int&)>* task
) {
  assert(threadNumber >= 0 && threadNumber < numberOfThreads);
  assert(isThreadPoolAlive == true);
  threadObjects[threadNumber]->assign(task);
}

void ThreadPool::joinThread(const unsigned int& threadNumber) {
  assert(threadNumber >= 0 && threadNumber < numberOfThreads);
  assert(isThreadPoolAlive);
  threadObjects[threadNumber]->join();
}

ThreadPool::ThreadObject::ThreadObject(const unsigned int& threadNumber)
  :
  isThreadPoolAlive(true),
  threadNumber(threadNumber),
  mutex(),
  lock(std::unique_lock<std::mutex>(mutex))
{
  thread = std::make_unique<std::thread>(mainLoopRefrence);
}

void ThreadPool::ThreadObject::assign(std::function<void(const unsigned int&)>* task) {
  assert(isThreadPoolAlive);
  tasks.push(task);
  condition.notify_one();
}

void ThreadPool::ThreadObject::join() {
  assert(isThreadPoolAlive);
  thread->join();
}

void ThreadPool::ThreadObject::notifyThreadPoolIsDead() {
  isThreadPoolAlive = false;
  thread->detach();
}

const unsigned int& ThreadPool::getNumberOfAvailableThreads() {
  return numberOfThreads;
}

bool ThreadPool::ThreadObject::sleepCondition() {
  return !tasks.empty();
}

void ThreadPool::ThreadObject::mainLoop() {
  while (isThreadPoolAlive)
  {
    condition.wait(lock, sleepConditionRefrence);
    if (tasks.front() != nullptr) {
      (*tasks.front())(threadNumber);
    }
    tasks.pop();
  }
}