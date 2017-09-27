#include <QCoreApplication>
#include <iostream>
#include <deque>
#include <typeinfo>
#include <QObject>
#include <functional>
#include <memory>
#include <QList>
#include <QTimer>
#include "boost/type_index.hpp"
#include <mutex>
#include <thread>
#include <map>
#include <unordered_map>
#include <type_traits>
#include <QDebug>
#include <QFile>
#include <fstream>
#include <QElapsedTimer>
#include <set>
#include <chrono>
#include <utility>
#include <future>

/// this test application is devoted to testing of new c++ feature
/// after Scott Meyers book
///
/// Chapter6: Concurrency API
///

using namespace std::literals;

template<typename T>
void showTypeBoost(T&& param)
{
    using boost::typeindex::type_id_with_cvr;

    // show T
    std::cout << "T =   "
              << type_id_with_cvr<T>().pretty_name()
              << "\n";

    std::cout << "param =   "
              << type_id_with_cvr<decltype(param)>().pretty_name()
              << "\n";
}

class Investment
{
public:
    int x()const{return m_x;}
private:
    int m_x{1};
};

/// Item 36: Specify std::launch::async if asynchronicity is essential
///
template <typename F, typename... Ts>
inline
//std::future<std::result_of_t<F(Ts...)> >
auto
reallyAsync(F&& f, Ts&&... params)
{
    return std::async(std::launch::async,
                      std::forward<F>(f),
                      std::forward<Ts>(params)...);
}

/// Item 37: Make std::threads unjoinable on all parts
///
class ThreadRAII
{
public:
    enum class DtorAction{ join, detach };
    ThreadRAII(std::thread&& t, DtorAction a) : action(a), t(std::move(t)) {}
    ~ThreadRAII()
    {
        if(t.joinable()){
            if(action == DtorAction::join){
                t.join();
            }else{
                t.detach();
            }
        }
    }
    ThreadRAII(ThreadRAII&&) = default;
    ThreadRAII& operator= (ThreadRAII&&) = default;

    std::thread& get() {return t;}

private:
    DtorAction action;
    std::thread t;
};







int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

/// Item 35: Prefer task based programming to thread based
///
    if(true)
    {
        std::cout <<  std::endl << std::endl << std:: endl
                   << "Item 35: Prefer task based programming to thread based"
                   << std::endl << std::endl;
        int i = std::atomic<int>(0);
        auto func1 = [&i](){};
        auto func2 = [&i](){std::cout << "in func2. i = " << i++ << std::endl;};
        auto thread1 = std::thread(func1); /// thread based approach
        auto async = std::async(func2);   /// task based approach
//        std::vector<std::thread> threads(1000);
//        try{
//            for(int j = 0; j < 1000; j++)
//            {
//                threads[j] = std::thread(func1);
//            }
//        }
//        catch(std::system_error)
//        {
//            std::cout << "too many system threads!" << std::endl;
//        }

        thread1.join();
    }

/// Item 36: Specify std::launch::async if asynchronicity is essential
///
    if(true)
    {
        std::cout <<  std::endl << std::endl << std:: endl
                   << "Item 36: Specify std::launch::async if asynchronicity is essential"
                   << std::endl << std::endl;
        int i = std::atomic<int>(0);
        auto func1 = [&i](){std::cout << "in func1. i = " << i++ << std::endl;};
        auto func2 = [&i](){std::cout << "in func2. i = " << i++ << std::endl;};
        std::cout << std::endl << "std::async" << std::endl;
        for(int i = 0; i < 10; i++)
        {
            auto async = std::async(func2);
            async.get();
        }
        std::cout << std::endl << "std::thread join" << std::endl;
        for(int i = 0; i < 10; i++)
        {
            auto thread = std::thread(func2);
            thread.join();
        }
        std::cout << std::endl << "std::thread detach" << std::endl;
        for(int i = 0; i < 10; i++)
        {
            auto thread = std::thread(func2);
            thread.detach();
        }
        auto async1 = std::async(func1);
//        while(async1.wait_for(100ms) !=
//              std::future_status::ready)
//        {
//            /// there is potentially infinite loop
//        }
//        std::cout << std::endl << "end of loop!" << std::endl;

        /// but it's ok, because func1 be done exactly in different loop
        ///
        auto async2 = std::async(std::launch::async, func1);
        while(async2.wait_for(100ms) !=
              std::future_status::ready)
        {
            /// there is no potentially infinite loop
        }
        std::cout << std::endl << "end of loop!" << std::endl;

        /// another solution
        ///
        auto func3 = [](){std::this_thread::sleep_for(2s);};
        auto async3 = std::async(func3);
        if(async3.wait_for(100ms) == std::future_status::deferred)
        {
            async3.get();
        }
        else
        {
            std::cout << "task isn't defered!" << std::endl;
        }
        std::cout << "after condition!" << std::endl;
        reallyAsync(func3);
        std::cout << "after really async!" << std::endl;
    }

/// Item 37: Make std::threads unjoinable on all parts
///
    if(true)
    {
        std::cout <<  std::endl << std::endl << std:: endl
                   << "Item 37: Make std::threads unjoinable on all parts"
                   << std::endl << std::endl;
        constexpr auto tenMillions = 10'000'000;
        //auto thread1 = std::thread([](){}); /// program will be terminated because of destructor of std::thread is called when it is joinable
        std::cout << "program continue to work" << std::endl << std::endl;
        auto thread2 = std::thread([](){std::cout << "inside thread 2" << std::endl;});
        auto thread3 = std::thread([](){std::cout << "inside thread 3" << std::endl;});
        ThreadRAII raii1(std::move(thread2), ThreadRAII::DtorAction::join);
        ThreadRAII raii2(std::move(thread3), ThreadRAII::DtorAction::detach);
    }

/// Item 38: Be aware of varying thread handle destructor behavior
///
    if(true)
    {
        std::cout <<  std::endl << std::endl << std:: endl
                   << "Item 38: Be aware of varying thread handle destructor behavior"
                   << std::endl << std::endl;
        auto func1 = [](){std::cout << "inside func1" << std::endl; return 1;};
        std::packaged_task<int()>pt(func1);
        auto fut = pt.get_future();
        std::thread t(std::move(pt));
        t.join();
        std::cout << "end of item 38" << std::endl;
    }

/// Item 39: Consider void futures for one-shot event communication
///
    if(true)
    {
        std::cout <<  std::endl << std::endl << std:: endl
                   << "Item 39: Consider void futures for one-shot event communication"
                   << std::endl << std::endl;
        std::condition_variable cv;

        std::cout << std::endl << "Variant with condvar" << std::endl;
        auto detectingTask = [cv = &cv](){
            for(int i = 0; i < 10; i++)
            {
                std::cout << i << std::endl;
                if(i >= 5)
                {
                    cv->notify_one();
                }
                std::this_thread::sleep_for(300ms);
            }
        };

        auto reactingTask = [cv = &cv](){
            {
                std::mutex m;
                std::unique_lock<std::mutex> lk(m);
                cv->wait(lk);
                std::cout << "reacting task is notified!" << std::endl;
            }
        };
        auto thread1 = std::thread(detectingTask);
        auto thread2 = std::thread(reactingTask);
        thread1.join();
        thread2.join();

        std::cout << std::endl << "variant with boolean flag" << std::endl;
        std::atomic<bool> flag{false};
        auto detector = [flag = &flag](){
            for(int i = 0; i < 10; i++)
            {
                std::cout << "in detector: " << i << std::endl;
                if(i == 5)
                {
                    flag->store(true);
                    std::cout << "detector: flag was triggered!" << std::endl;
                }
                std::this_thread::sleep_for(300ms);
            }
        };
        auto reactor = [flag = &flag](){
            while(!flag->load())
            {
               std::cout << "reactor: flag no!" << std::endl;
               std::this_thread::sleep_for(100ms);
            }
            std::cout << "reactor: flag was triggered!" << std::endl;
        };
        auto asyncDetector = std::async(std::launch::async,detector);
        auto asyncReactor = std::async(std::launch::async,reactor);
        asyncDetector.get();
        asyncReactor.get();

        std::cout << std::endl << "variant with combined condvar and boolean flag" << std::endl;
        bool flag1{false};
        std::condition_variable cv1;
        auto detector1 = [cv = &cv1, flag = &flag1](){
            std::mutex m1;
            for(int i = 0; i < 10; i++)
            {
                std::cout << "in detector: " << i << std::endl;
                if(i == 5)
                {
                    {
                        std::lock_guard<std::mutex> g1(m1);
                        *flag = true;
                        std::cout << "detector: flag was triggered!" << std::endl;
                    }
                    cv->notify_one();
                }
                std::this_thread::sleep_for(300ms);
            }
        };
        auto reactor1 = [cv = &cv1, flag = &flag1](){
            std::mutex m1;
            std::unique_lock<std::mutex> lk(m1);
            cv->wait(lk,[flag = flag]{return *flag;});
            std::cout << "reactor: flag was triggered!" << std::endl;
        };
        auto asyncDetector1 = std::async(std::launch::async,detector1);
        auto asyncReactor1 = std::async(std::launch::async,reactor1);
        asyncDetector1.get();
        asyncReactor1.get();


        std::cout << std::endl << "variant with future" << std::endl;
        std::promise<void> p;

        auto detector2 = [p = &p](){
            std::mutex m1;
            for(int i = 0; i < 10; i++)
            {
                std::cout << "in detector: " << i << std::endl;
                if(i == 5)
                {
                    {
                        p->set_value();
                        std::cout << "detector: flag was triggered!" << std::endl;
                    }
                }
                std::this_thread::sleep_for(300ms);
            }
        };
        auto reactor2 = [p = &p](){
            p->get_future().wait();
            std::cout << "reactor: flag was triggered!" << std::endl;
        };
        auto asyncDetector2 = std::async(std::launch::async,detector2);
        auto asyncReactor2 = std::async(std::launch::async,reactor2);
        asyncDetector2.get();
        asyncReactor2.get();

    }

/// Item 40: Use std::atomic for concurrency, volatile for special memory
///
    if(true)
    {
        std::cout <<  std::endl << std::endl << std:: endl
                   << "Item 40: Use std::atomic for concurrency, volatile for special memory"
                   << std::endl << std::endl;

    }

    return a.exec();
}
