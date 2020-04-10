#ifndef ThreadPool_Class
#define ThreadPool_Class

#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <vector>
#include <queue>

class ThreadPool
{

public:

  ThreadPool();

  ~ThreadPool();

  void assignTask(
    const unsigned int& threadNumber,
    std::function<void(const unsigned int&)>* task
  );

  void joinThread(const unsigned int& threadNumber);

  const unsigned int& getNumberOfAvailableThreads();

  class ThreadObject{

  public:

    ThreadObject(const unsigned int& threadNumber);

    void mainLoop();

    void assign(
      std::function<void(const unsigned int&)>* task
    );

    void join();

    void notifyThreadPoolIsDead();

    bool isThreadPoolAlive;

  private:

    unsigned int threadNumber;

    std::mutex mutex;

    std::unique_lock<std::mutex> lock;

    std::queue<std::function<void(const unsigned int&)>*> tasks;

    std::condition_variable condition;

    std::unique_ptr<std::thread> thread;

    bool sleepCondition();

    std::function<bool(void)> sleepConditionRefrence = std::bind(
      &ThreadPool::ThreadObject::sleepCondition, 
      this
    );

    std::function<void(void)> mainLoopRefrence = std::bind(
      &ThreadPool::ThreadObject::mainLoop,
      this
    );

  };

private:

  std::vector<std::unique_ptr<ThreadObject>> threadObjects;

  bool isThreadPoolAlive;

  unsigned int numberOfThreads = 0;

};

#endif // !ThreadPool_Class
