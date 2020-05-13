#include "./ThreadPool.h"

#include "../../Constants.h"
#include "../log/Logger.h"

//TODO Add preference for using threads for applications//For example using threads is not efficient in android
//TODO CPU multi threading currently does not seem efficient on mobile devices
ThreadPool::ThreadPool()
{

  mainThreadId = std::this_thread::get_id();

#ifdef __DESKTOP__
  numberOfThreads = (unsigned int)(float(std::thread::hardware_concurrency()) / 2);
#endif // __DESKTOP__
#ifdef __MOBILE__
  //Cpu threads does not seem to work efficiently on mobiles
  numberOfThreads = 0;
#endif
  //numberOfThreads = 1;
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

bool ThreadPool::isMainThread() {
  return std::this_thread::get_id() == mainThreadId;
}

ThreadPool::~ThreadPool() {
  isThreadPoolActive = false;
}

void ThreadPool::assignTaskToAllThreads(
  std::function<void(const unsigned int&, void*)>* task,
  void* param
) {
  assert(std::this_thread::get_id() == mainThreadId);
  for (threadIndex = 0; threadIndex < numberOfThreads; threadIndex++) {
    assignTask(threadIndex, task, param);
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
  lock(mutex),
  parent(parent)
{
  thread = std::make_unique<std::thread>(mainLoopReference);
}

ThreadPool::ThreadObject::~ThreadObject() {
  thread->detach();
}

void ThreadPool::ThreadObject::assign(std::function<void(const unsigned int&,void*)>* function,void* param) {
  
  ThreadPool::ThreadObject::Task task;
  task.function = function;
  task.parameter = param;
  tasks.push(task);

  condition.notify_one();
}

const unsigned int& ThreadPool::getNumberOfAvailableThreads() const {
  return numberOfThreads;
}

bool ThreadPool::ThreadObject::awakeCondition() {
  return !tasks.empty();
}

void ThreadPool::ThreadObject::mainLoop() {
  while (parent.isThreadPoolActive)
  {

    condition.wait(lock, threadAwakeConditionReference);
    
    while (tasks.empty()==false) {
      try {
        auto& currentTask = tasks.front();
        if (currentTask.function != nullptr) {
          (*currentTask.function)(threadNumber, currentTask.parameter);
        }
      }
      catch (std::exception exception) {
        parent.exceptions.push(exception.what());
      }
      tasks.pop();
    };

  }
}

bool ThreadPool::mainThreadAwakeCondition() {
  for (auto& threadObject : threadObjects) {
    if (threadObject->tasks.empty() == false) {
      return false;
    }
  }
  return true;
}

void ThreadPool::waitForThreadsToFinish() {
  assert(std::this_thread::get_id() == mainThreadId);
  //When number of threads is 2 it means that platform only uses main thread
  if (isThreadPoolActive) {
    while (mainThreadAwakeCondition() == false) {
      for (auto& threadObject : threadObjects) {
        threadObject->condition.notify_one();
      }
      std::this_thread::sleep_for(std::chrono::nanoseconds(50));
    }
    assert(allThreadsTasksQueueIsEmpty()==true);
    while (exceptions.size() != 0)
    {
      Logger::log(exceptions.front());
      exceptions.pop();
    }
  }
}

bool ThreadPool::allThreadsTasksQueueIsEmpty() {
  for (auto& threadObject : threadObjects) {
    if (threadObject->tasks.empty() == false) {
      Logger::log("ThreadObject work is not finished");
      Logger::log("Tasks:" + std::to_string(threadObject->tasks.size()));
      return false;
    }
  }
  return true;
}