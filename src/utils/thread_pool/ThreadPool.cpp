#include "./ThreadPool.h"

#include "../log/Logger.h"

ThreadPool::ThreadPool()
  :
  isThreadPoolAlive(true),
  mainThreadMutex(),
  mainThreadLock(std::unique_lock<std::mutex>(mainThreadMutex))
{

  numberOfThreads = (unsigned int)(float(std::thread::hardware_concurrency())/2);
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
    
    for (threadIndex = 0; threadIndex < numberOfThreads; threadIndex++) {
      threadObjects.emplace_back(std::make_unique<ThreadObject>(threadIndex,*this));
    }

  }

}

ThreadPool::~ThreadPool() {
  isThreadPoolAlive = false;
}

void ThreadPool::assignTask(
  const unsigned int& threadNumber,
  std::function<void(const unsigned int&)>* task
) {
  assert(threadNumber >= 0 && threadNumber < numberOfThreads);
  assert(isThreadPoolAlive == true);
  threadObjects[threadNumber]->assign(task);
}

ThreadPool::ThreadObject::ThreadObject(const unsigned int& threadNumber, ThreadPool& parent)
  :
  threadNumber(threadNumber),
  mutex(),
  lock(std::unique_lock<std::mutex>(mutex)),
  parent(parent)
{
  thread = std::make_unique<std::thread>(mainLoopRefrence);
}

ThreadPool::ThreadObject::~ThreadObject() {
  thread->detach();
}

void ThreadPool::ThreadObject::assign(std::function<void(const unsigned int&)>* task) {
  assert(parent.isThreadPoolAlive);
  tasks.push(task);
  condition.notify_one();
}

const unsigned int& ThreadPool::getNumberOfAvailableThreads() {
  return numberOfThreads;
}

bool ThreadPool::ThreadObject::sleepCondition() {
  return !isBusy;
}

void ThreadPool::ThreadObject::mainLoop() {
  while (parent.isThreadPoolAlive)
  {
    condition.wait(lock, sleepConditionRefrence);
    conditionVariablesMutex.lock();
    isBusy = true;
    conditionVariablesMutex.unlock();
    while (tasks.empty() == false) {
      try {
        if (tasks.front() != nullptr) {
          (*tasks.front())(threadNumber);
        }
      }
      catch (std::exception exception) {
        parent.exceptions.push(exception.what());
      }
      tasks.pop();
    }
    conditionVariablesMutex.lock();
    isBusy = false;
    conditionVariablesMutex.unlock();
    parent.mainThreadCondition.notify_one();
  }
}

bool ThreadPool::mainThreadSleepCondition() {
  for (auto& threadObject : threadObjects) {
    if (threadObject->isBusy) {
      return false;
    }
  }
  return true;
}

void ThreadPool::waitForThreadsToFinish() {
  assert(numberOfThreads > 1);
  for (auto& threadObject : threadObjects) {
    threadObject->conditionVariablesMutex.lock();
    threadObject->conditionVariablesMutex.unlock();
  }
  mainThreadCondition.wait(mainThreadLock, mainThreadSleepConditionRefrence);
  while (exceptions.size() != 0)
  {
    Logger::log(exceptions.front());
    exceptions.pop();
  }
}