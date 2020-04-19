#ifndef ThreadPool_Class
#define ThreadPool_Class

#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <vector>
#include <queue>
#include <exception>

class ThreadPool
{

public:

  ThreadPool();

  ~ThreadPool();

  void autoAssignTask(
    std::function<void(const unsigned int&, void*)>* task,
    void* param
  );

  void assignTask(
    const unsigned int& threadNumber,
    std::function<void(const unsigned int&,void*)>* task,
    void* param
  );

  const unsigned int& getNumberOfAvailableThreads() const;

  void waitForThreadsToFinish();

  class ThreadObject{

  public:

    ThreadObject(const unsigned int& threadNumber,ThreadPool& parent);

    ~ThreadObject();

    void mainLoop();

    void assign(
      std::function<void(const unsigned int&, void* param)>* task,
      void* params
    );

    bool sleepCondition();

    ThreadPool& parent;

    unsigned int threadNumber;

    std::mutex mutex;

    std::unique_lock<std::mutex> lock;

    std::queue<std::function<void(const unsigned int&, void* )>*> tasks;

    std::queue<void*> parameters;

    std::condition_variable condition;

    std::unique_ptr<std::thread> thread;

    std::function<bool(void)> sleepConditionReference = std::bind(
      &ThreadPool::ThreadObject::sleepCondition, 
      this
    );

    std::function<void(void)> mainLoopReference = std::bind(
      &ThreadPool::ThreadObject::mainLoop,
      this
    );

    std::mutex conditionVariablesMutex;

    bool isBusy = false;

  };

private:
  
  bool mainThreadSleepCondition();

  std::function<bool(void)> mainThreadSleepConditionReference = std::bind(
    &ThreadPool::mainThreadSleepCondition,
    this
  );

  std::vector<std::unique_ptr<ThreadObject>> threadObjects;

  bool isThreadPoolActive = true;

  unsigned int numberOfThreads = 0;

  unsigned int threadIndex = 0;

  std::mutex mainThreadMutex;

  std::unique_lock<std::mutex> mainThreadLock;

  std::condition_variable mainThreadCondition;

  std::queue<std::string> exceptions;

  unsigned int currentThreadIndex = 0;

  std::thread::id mainThreadId;

};

#endif // !ThreadPool_Class
