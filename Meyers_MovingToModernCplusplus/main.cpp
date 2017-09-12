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

/// this test application is devoted to testing of new c++ feature
/// after Scott Meyers book
///
/// Chapter3: Moving to Modern C++
///

template<typename T>
void showTypeBoost(T& param)
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

class ClassWithDefaultValues
{
public:

    int x() const
    {
        return m_x;
    }

    float f() const
    {
        return m_f;
    }

private:
    int m_x{3};
    float m_f{1.1111};
};

/// item 8
///
template<typename FuncType,
         typename MuxType,
         typename PtrType>
decltype(auto) lockAndCall(FuncType func,
                           MuxType& mutex,
                           PtrType ptr)
{
    std::lock_guard<std::mutex> g(mutex);
    return func(ptr);
}

int f1(std::shared_ptr<QObject> ob)
{
    std::cout << "inside int f1(std::shared_ptr<QObject> ob)" << std::endl;
    return 1;
}

double f2(std::unique_ptr<QObject> ob)
{
    std::cout << "inside double f2(std::unique_ptr<QObject> ob)" << std::endl;
    return 1.1;
}

bool f3(QObject *ob)
{
    std::cout << "inside bool f3(QObject *ob)" << std::endl;
    return true;
}

/// Item 9;
/// alias declaration
///
using UPtrMapSS = std::unique_ptr<std::unordered_map<std::string, std::string> >;
using Fp = void(*)(int,const std::string&);
/// alias template
///
template<typename T>
using MyVector = std::vector<T>;

template<typename T>
using removeConstAndRef = std::remove_const_t<std::remove_reference_t<T> >;


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

/// Item 7: Distinguish between () and {} when creating objects
///
    if(true)
    {
        int x0 = 0;
        int y0(0);
        int z0 = {0};
        int w0 = {0};
        ClassWithDefaultValues o1;  /// default ctor
        ClassWithDefaultValues o2 = o1;  /// copy ctor
        o1 = o2;   /// assegnment operator
        std::vector<int> v{1, 2, 3}; /// initialize container with values
        std::cout << "m_x: " << o2.x() << "  m_f: " << o2.f() << std::endl;
        std::atomic<int> ai1{0}; /// atomic objects can be initialized by braces
        std::atomic<int> ai3(0); /// and parenthness
        //std::atomic<int> ai2 = 0; /// but not assignment
        double x, y, z;
        x = y = z = 1.2341;
        double w1{x+y+z}; /// work
        int w2{x+y+z}; /// not work because of narrowing conversions
        int w3 = x+y+z;  /// no narrowing conversions
        int w4(x+y+z); /// no narrowing conversions
        ClassWithDefaultValues o4{};
    }

/// Item 8: Prefer nullptr to 0 and NULL
///
    if(true)
    {
        std::cout << std::endl << std::endl << std:: endl << "Item 8: Prefer nullptr to 0 and NULL" << std::endl << std::endl;
        QObject *o1 = nullptr;
        auto o2 = o1;
        if(o2 == nullptr)
        {
            std::cout << "o2 is null pointer!" << std::endl;
        }
        else
        {
            std::cout << "o2 is good pointer!" << std::endl;
        }
        std::mutex fm1, fm2, fm3;
//        auto result1 = lockAndCall(f1,fm1,0);  /// type error. 0 - integer
//        auto result2 = lockAndCall(f2,fm2,NULL);   /// type error. NULL - integer
        auto result1 = lockAndCall(f1,fm1,nullptr);  /// all good
        auto result2 = lockAndCall(f2,fm2,nullptr);  /// all good
        auto result3 = lockAndCall(f3,fm3,nullptr);  /// all good
    }

/// Item 9: Prefer alias declarations to typdefs
///
    if(true)
    {
        std::cout <<  std::endl << std::endl << std:: endl << "Item 9: Prefer alias declarations to typdefs" << std::endl << std::endl;
        MyVector<QObject*> myVector{new QObject, new QObject, new QObject};
        std::cout << "vector contains " << myVector.size() << " elements" << std::endl;

        /// type traits
        ///
        const int& x = 25;
        std::cout << "type of x before type trait: " << std::endl;
        showTypeBoost(x);
        removeConstAndRef<decltype(x)> y;
        std::cout << "type of y after type traits " << std::endl;
        showTypeBoost(y);
    }

/// Item 10: Prefer scoped enums to unscoped enums
///
    if(true)
    {
        std::cout <<  std::endl << std::endl << std:: endl
                   << "Item 10: Prefer scoped enums to unscoped enums"
                   << std::endl << std::endl;

    }


    return a.exec();
}

