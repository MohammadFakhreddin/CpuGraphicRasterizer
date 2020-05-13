#ifndef ThreadPool_Class
#define ThreadPool_Class

#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <vector>
#include <queue>
#include <exception>
#include <atomic>

class ThreadPool
{

public:

  ThreadPool();

  ~ThreadPool();

  bool isMainThread();

  void assignTaskToAllThreads(
    std::function<void(const unsigned int&, void*)>* function,
    void* param
  );

  void assignTask(
    const unsigned int& threadNumber,
    std::function<void(const unsigned int&,void*)>* function,
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
      std::function<void(const unsigned int&, void* param)>* function,
      void* params
    );

    bool awakeCondition();

    ThreadPool& parent;

    unsigned int threadNumber;

    std::mutex mutex;

    std::unique_lock<std::mutex> lock;

    struct Task {
      std::function<void(const unsigned int&, void*)>* function;
      void* parameter;
    };

    std::queue<Task> tasks;

    std::condition_variable condition;

    std::unique_ptr<std::thread> thread;

    std::function<bool(void)> threadAwakeConditionReference = std::bind(
      &ThreadPool::ThreadObject::awakeCondition, 
      this
    );

    std::function<void(void)> mainLoopReference = std::bind(
      &ThreadPool::ThreadObject::mainLoop,
      this
    );

  };

  bool allThreadsTasksQueueIsEmpty();

private:
  
  bool mainThreadAwakeCondition();

  std::function<bool(void)> mainThreadAwakeConditionReference = std::bind(
    &ThreadPool::mainThreadAwakeCondition,
    this
  );

  std::vector<std::unique_ptr<ThreadObject>> threadObjects;

  bool isThreadPoolActive = true;

  unsigned int numberOfThreads = 0;

  unsigned int threadIndex = 0;

  std::queue<std::string> exceptions;

  std::thread::id mainThreadId;

};

#endif // !ThreadPool_Class
