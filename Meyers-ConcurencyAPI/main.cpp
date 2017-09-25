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
        auto func1 = [&i](){std::cout << "in func1. i = " << i++ << std::endl;};
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

    }


    return a.exec();
}
