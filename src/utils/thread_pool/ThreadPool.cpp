#include "./ThreadPool.h"

#include "../../Constants.h"
#include "../log/Logger.h"

//TODO Add preference for using threads for applications//For example using threads is not efficient in android
//TODO CPU multi threading currently does not seem efficient on mobile devices
ThreadPool::ThreadPool()
  :
  mainThreadMutex(),
  mainThreadLock(std::unique_lock<std::mutex>(mainThreadMutex))
{
#ifdef __DESKTOP__
  numberOfThreads = (unsigned int)(float(std::thread::hardware_concurrency()) / 2);
#endif // __DESKTOP__
#ifdef __MOBILE__
  //Cpu threads does not seem to work efficiently on mobiles
  numberOfThreads = 0;
#endif
  if (numberOfThreads == 0) {
    numberOfThreads = 1;
  }

  Logger::log("Maximum number of threads: " + std::to_string(std::thread::hardware_concurrency()));
  Logger::log("Initiating thread pool with " + std::to_string(numberOfThreads) + " threads.");

  if (numberOfThreads < 2) {

    Logger::log("This system cannot use threads");
    isThreadPoolActive = false;
  
  } else {

    isThreadPoolActive = true;

    threadObjects.reserve(numberOfThreads);
    
    for (threadIndex = 0; threadIndex < numberOfThreads; threadIndex++) {
      threadObjects.emplace_back(std::make_unique<ThreadObject>(threadIndex,*this));
    }

  }

}

ThreadPool::~ThreadPool() {
  isThreadPoolActive = false;
}

void ThreadPool::assignTask(
  const unsigned int& threadNumber,
  std::function<void(const unsigned int&)>* task
) {
  assert(threadNumber >= 0 && threadNumber < numberOfThreads);
  if (numberOfThreads > 1) {
    threadObjects[threadNumber]->assign(task);
  }
  else
  {
    if (task != nullptr) {
      (*task)(threadNumber);
    }
  }
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
  while (parent.isThreadPoolActive)
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
  //When number of threads is 2 it means that platform only uses main thread
  if (isThreadPoolActive) {
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
}