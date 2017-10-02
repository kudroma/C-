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
/// Chapter8: Tweaks
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

    Investment()=default;
    Investment(const Investment&){std::cout << "inside copy ctor!" << std::endl;}
    Investment(Investment&&) = default;

private:
    int m_x{1};
};

/// Item 42: Consider emplacement instead of insertion
///
class ForExplicitInterpretation1
{
public:
    ForExplicitInterpretation1(int i = 0){}
};

class ForExplicitInterpretation2
{
public:
    explicit ForExplicitInterpretation2(int i = 0){}
};

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

/// Item 41: Consider pass by value for copyable parameters that are cheap to move and always copied
///
    if(true)
    {
        std::cout <<  std::endl << std::endl << std:: endl
                   << "Item 35: Prefer task based programming to thread based"
                   << std::endl << std::endl;

    }

/// Item 42: Consider emplacement instead of insertion
///
    if(true)
    {
        std::cout <<  std::endl << std::endl << std:: endl
                   << "Item 42: Consider emplacement instead of insertion"
                   << std::endl << std::endl;

        ForExplicitInterpretation1 object1 = 1;
        //ForExplicitInterpretation2 object2 = 2; /// error constructor with integer is explicit!
    }

    Investment inv;

    Investment inv2(inv);

    auto&& inv3 = Investment();

    auto func = [](auto&& inv){inv;};
    func(inv);
    func(inv2);
    func(inv3);

    return a.exec();
}
