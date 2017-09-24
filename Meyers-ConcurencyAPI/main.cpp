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

/// this test application is devoted to testing of new c++ feature
/// after Scott Meyers book
///
/// Chapter6: Lambdas
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

/// Item 34: Prefer lambdas to std::bind
///
    if(true)
    {
        std::cout <<  std::endl << std::endl << std:: endl
                   << "Item 34: Prefer lambdas to std::bind"
                   << std::endl << std::endl;

    }


    return a.exec();
}
