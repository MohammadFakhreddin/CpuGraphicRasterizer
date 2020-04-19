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

  mainThreadId = std::this_thread::get_id();

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

void ThreadPool::autoAssignTask(
  std::function<void(const unsigned int&, void*)>* task,
  void* param
) {

  assert(std::this_thread::get_id() == mainThreadId);
  
  assignTask(currentThreadIndex, task, param);
  currentThreadIndex++;
  if (currentThreadIndex > numberOfThreads) {
    currentThreadIndex = 0;
  }

}

void ThreadPool::assignTask(
  const unsigned int& threadNumber,
  std::function<void(const unsigned int&,void*)>* task,
  void* param
) {
  assert(std::this_thread::get_id() == mainThreadId);
  assert(threadNumber >= 0 && threadNumber < numberOfThreads);

  if (numberOfThreads > 1) {
    threadObjects[threadNumber]->assign(task, param);
  }
  else
  {
    if (task != nullptr) {
      (*task)(threadNumber, param);
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
  thread = std::make_unique<std::thread>(mainLoopReference);
}

ThreadPool::ThreadObject::~ThreadObject() {
  thread->detach();
}

void ThreadPool::ThreadObject::assign(std::function<void(const unsigned int&,void*)>* task,void* param) {
  tasks.push(task);
  parameters.push(param);

  condition.notify_one();
}

const unsigned int& ThreadPool::getNumberOfAvailableThreads() const {
  return numberOfThreads;
}

bool ThreadPool::ThreadObject::sleepCondition() {
  return !isBusy;
}

void ThreadPool::ThreadObject::mainLoop() {
  while (parent.isThreadPoolActive)
  {
    condition.wait(lock, sleepConditionReference);
    conditionVariablesMutex.lock();
    isBusy = true;
    conditionVariablesMutex.unlock();
    while (tasks.empty() == false) {
      try {
        if (tasks.front() != nullptr) {
          (*tasks.front())(threadNumber, parameters.front());
        }
      }
      catch (std::exception exception) {
        parent.exceptions.push(exception.what());
      }
      parameters.pop();
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
    mainThreadCondition.wait(mainThreadLock, mainThreadSleepConditionReference);
    while (exceptions.size() != 0)
    {
      Logger::log(exceptions.front());
      exceptions.pop();
    }
  }
}