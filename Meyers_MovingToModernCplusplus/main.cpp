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

/// Item 10. Function for return of enumerator type
///
template<typename E>
constexpr auto
  toUtype(E enumerator) noexcept
{
    return static_cast<std::underlying_type_t<E> >(enumerator);
}

/// Item 11. Prefer deleted functions to undefined private ones.
///
template<typename T>
void sayAboutPointer(T* ptr)
{
    std::cout << "I can say about ptr" << std::endl;
}
template<> void sayAboutPointer(void* ptr) = delete;
template<> void sayAboutPointer(char* ptr) = delete;


/// Item 12. Declare overriding functions override
///
/// class with reference qualifiers
class classWithOverriding
{
public:
    void doWork() &
    {
        std::cout << "this version of doWork is for lvalues" << std::endl;
    }

    void doWork() &&
    {
        std::cout << "this version of doWork is for rvalues" << std::endl;
    }
    virtual void f1() &
    {
        std::cout << "this is base function" << std::endl;
    }

    virtual void f2() &
    {

    }
};
classWithOverriding getObject()
{
    return *(new classWithOverriding());
}
class derivedClassWithOverriding : public classWithOverriding
{
public:
//    virtual void f1() && override
//    {
//        std::cout << "this is wrongly override function in derived class" << std::endl;
//    } /// error because conditions for overriding is not good
    void f2() & override
    {

    }
};





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

         enum UnscopedEnum {black, white, red};
        //auto white = 1;  /// error. Unscoped enums capture names
        enum class  ScopedEnum {black, white, red, green};
        auto green = 1; /// all good. Scoped enums don't capture names
        auto c = ScopedEnum::black;
        //auto c = black;  /// error. black of ScopedEnum doesn't exist outside its scope
        if(black < 14.5) // &&
               // ScopedEnum::black < 14.3) /// error. we can't compare scoped enum values with float
        {
          //  std::cout << "Ai-ai-ai" << std::endl;
        }
        //enum Color;  /// unscoped enums can't be forward declared
        enum class Color;
        enum class Status: std::uint32_t {status}; /// set underlying enum type for Status
        enum UnscopedStatus: std::uint8_t; /// we can do it for unscoped enums too
        /// using unscoped enums to get elements from tuples
        ///
        using UserInfo = std::tuple<std::string,std::string,std::size_t>;
        enum UserInfoFields {uName, uEmail, uReputation};
        UserInfo uinfo = std::make_tuple(std::string("Raman"), std::string("kudroma@list.ru"), 5);
        auto name = std::get<uName>(uinfo);
        auto email = std::get<uEmail>(uinfo);
        auto reputation = std::get<uReputation>(uinfo);
        std::cout << name << " " << email << " " << reputation << std::endl;
        std::cout << "underlying type for ScopedEnum: " << std::endl;
        showTypeBoost(toUtype(ScopedEnum::black));
        std::cout << std::endl;
        std::cout << "underlying type for UnscopedEnum: " << std::endl;
        showTypeBoost(toUtype(black));
        std::cout << std::endl;
        std::cout << "underlying type for std::uint32_t Status enum: " << std::endl;
        showTypeBoost(toUtype(Status::status));
        std::cout << std::endl;
    }

/// Item 11: Prefer deleted functions to private undefined ones
///
    if(true)
    {
        std::cout <<  std::endl << std::endl << std:: endl
                   << "Item 11: Prefer deleted functions to private undefined ones"
                   << std::endl << std::endl;
        class ClassWithDeletedFuncions
        {
        public:
            ClassWithDeletedFuncions()
            {
                std::cout << "create ClassWithDeletedFunctions object";
            }

            ClassWithDeletedFuncions(const ClassWithDeletedFuncions&) = delete;
            ClassWithDeletedFuncions& operator=(const ClassWithDeletedFuncions&) = delete;
        };

        ClassWithDeletedFuncions object1;
        // ClassWithDeletedFuncions object2 = object1; /// error because assignment operator is deleted
        int* ptr1 = nullptr;
        void* ptr2 = nullptr;
        char* ptr3 = nullptr;
        sayAboutPointer(ptr1);
//        sayAboutPointer(ptr2);  /// void* ptr function realization is deleted
//        sayAboutPointer(ptr3);  /// char* ptr function realization is deleted
    }

/// Item 12: Declare overriding functions override
///
    if(true)
    {
        std::cout <<  std::endl << std::endl << std:: endl
                   << "Item 12: Declare overriding functions override"
                   << std::endl << std::endl;

        classWithOverriding object1;
        object1.doWork(); /// call member function version for lvalue
        getObject().doWork(); /// call member function version for rvalue
    }

/// Item 13: Prefer const_iterators to iterators
///
    if(true)
    {
        std::cout <<  std::endl << std::endl << std:: endl
                   << "Item 13: Prefer const_iterators to iterators"
                   << std::endl << std::endl;
        std::vector<int> years {1983,1986,2011,2014,2017};
        for(int i = 0; i < years.size(); i++)
        {
            std::cout << years.at(i) << " ";
        }
        std::cout << std::endl;
        auto it = std::find(years.cbegin(),years.cend(),1983);
        years.insert(it,1998);
        for(int i = 0; i < years.size(); i++)
        {
            std::cout << years.at(i) << " ";
        }
        std::cout << std::endl;
        std::cbegin(years); /// compiles in C++14 but not in C++11
    }

/// Item 14: Declare functions noexcept if they won't emit exceptions
///
    if(true)
    {
        std::cout <<  std::endl << std::endl << std:: endl
                   << "Item 14: Declare functions noexcept if they won't emit exceptions"
                   << std::endl << std::endl;

    }



    return a.exec();
}

