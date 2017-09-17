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

/// Item 14: Declare functions noexcept if they won't emit exceptions
///
void withNoExceptionFunction() noexcept
{
    std::cout << "This function has never emit exception" << std::endl;
}
void status()
{
    std::cout << "I can emit exception! And I'm not catch by compiler!" << std::endl;
}
void compilerNotDetectableProgblem() noexcept
{
    std::cout << "This function may emit exception but compiler doesn't see this!" << std::endl;
    status();
}

/// Item 15: Use constexpr whenever possible
///
/// this function is not valid in C++11. Because constexpr functions here must contain only one statement with return.
///
constexpr int pow(float base, int exp)
{
    auto result = 1;
    for(int i = 0; i < exp; i++) result *= base;
    return result;
}
/// class objects of which can be constexpr
///
class Point
{
public:

    constexpr Point(double x = 0, double y = 0) noexcept :
        m_x(x),
        m_y(y)
    {}

    constexpr double x() const
    {
        return m_x;
    }
    constexpr void setX(double x)
    {
        m_x = x;
    }

    constexpr double y() const
    {
        return m_y;
    }
    constexpr void setY(double y)  /// in C++11 setters can't be constexpr, because they are not const and void is no literal. In C++14 it is possible.
    {
        m_y = y;
    }

private:
    double m_x, m_y;
};
/// constexpr function using constexpr user-defined type Point
///
constexpr Point midPoint(Point point1, Point point2)
{
    return Point((point1.x() + point2.x())/2,(point1.y() + point2.y())/2);
}
/// constexpr function using setters of constexpr object
///
constexpr Point reflection(const Point& point)
{
    Point result;
    result.setX(-point.x());
    result.setY(-point.y());
    return result;
}


/// Item 16: Make const member functions thread safe
///
/// class with caching and mutable
///
class classWithCaching
{
public:
    virtual double computeValue() const
    {
        std::cout << "value before computing: " << cachedValue << std::endl;

        if(!isCached)
        {
            isCached = true;
            cachedValue = 19.2;
            std::cout << "value is actually computed and is equeal: " << cachedValue << std::endl;
        }
        else
        {
            std::cout << "value is get from cache!" << std::endl;
        }
        return cachedValue;
    }
protected:
    mutable bool isCached {false};
    mutable double cachedValue {0};
};
classWithCaching object_caching;
void callFromThread12()
{
    object_caching.computeValue();
}
class classWithCachingAndMutex : public classWithCaching
{
public:
    virtual double computeValue() const override
    {
        std::lock_guard<std::mutex> g(m);
        return classWithCaching::computeValue();
    }
protected:
    mutable std::mutex m;
};
classWithCachingAndMutex object_caching_mutex;
void callFromThread34()
{
    object_caching_mutex.computeValue();
}
class HowManyTimes
{
public:
    virtual void howManyTimes() const
    {
        std::cout << m_callCount << std::endl;
        m_callCount++;
    }
private:
    mutable int m_callCount {1};
};
HowManyTimes howManyTimes;
void callFromThread5()
{
    for(int i = 0; i < 20; i++)
    {
        std::cout << "from thread 5 call - ";
        howManyTimes.howManyTimes();
    }
}
void callFromThread6()
{
    for(int i = 0; i < 20; i++)
    {
        std::cout << "from thread 6 call - ";
        howManyTimes.howManyTimes();
    }
}
class HowManyTimesWithAtomic : public HowManyTimes
{
public:
    virtual void howManyTimes() const override
    {
        std::cout << callCount << std::endl;
        callCount++;
    }
private:
    mutable std::atomic<unsigned> callCount {1};
};
HowManyTimesWithAtomic howManyTimesWithAtomic;
void callFromThread7()
{
    for(int i = 0; i < 20; i++)
    {
        std::cout << "from thread 7 call - ";
        howManyTimesWithAtomic.howManyTimes();
    }
}
void callFromThread8()
{
    for(int i = 0; i < 20; i++)
    {
        std::cout << "from thread 8 call - ";
        howManyTimesWithAtomic.howManyTimes();
    }
}

/// Item 17: Understand special member functino generation
/// 
class WidgetWithAllDefaultOperations
{
public:
    WidgetWithAllDefaultOperations() = default;
    WidgetWithAllDefaultOperations(WidgetWithAllDefaultOperations&& rhs){}   /// move constructor
    WidgetWithAllDefaultOperations& operator=(WidgetWithAllDefaultOperations&& rhs){} /// move assignment operator
    WidgetWithAllDefaultOperations(WidgetWithAllDefaultOperations& rhs) = default;
    WidgetWithAllDefaultOperations& operator=(WidgetWithAllDefaultOperations& rhs) = default;
    virtual ~WidgetWithAllDefaultOperations() = default;
};
class WidgetWithCopyConstructorOnly
{
public:
    WidgetWithCopyConstructorOnly(){}
    WidgetWithCopyConstructorOnly(WidgetWithCopyConstructorOnly& rhs)
    {
        std::cout << "WidgetWithCopyConstructorOnly - inside copy constructor" << std::endl;
    }
    virtual ~WidgetWithCopyConstructorOnly()
    {
        std::cout << "WidgetWithCopyConstructorOnly - inside destructor" << std::endl;
    }
};

class WidgetWithMoveConstructorOnly
{
public:
    WidgetWithMoveConstructorOnly() = default;
    WidgetWithMoveConstructorOnly(WidgetWithMoveConstructorOnly&& rhs)
    {
        std::cout << "WidgetWithMoveConstructorOnly - inside move constructor" << std::endl;
    }
    WidgetWithMoveConstructorOnly& operator=(WidgetWithMoveConstructorOnly& rhs) = default;
    virtual ~WidgetWithMoveConstructorOnly()
    {
        std::cout << "WidgetWithMoveConstructorOnly - inside destructor" << std::endl;
    }
};

class WidgetWithAllOperationsDeclared
{
public:
    WidgetWithAllOperationsDeclared() = default;
    WidgetWithAllOperationsDeclared(WidgetWithAllOperationsDeclared&& rhs)
    {
        std::cout << "WidgetWithAllOperationsDeclared - inside move constructor" << std::endl;
    }
    WidgetWithAllOperationsDeclared& operator=(WidgetWithAllOperationsDeclared& rhs) = default;
    WidgetWithAllOperationsDeclared& operator=(WidgetWithAllOperationsDeclared&& rhs)
    {
        std::cout << "here!";
    }
    WidgetWithAllOperationsDeclared(WidgetWithAllOperationsDeclared& rhs)
    {
        std::cout << "WidgetWithAllOperationsDeclared - inside copy constructor" << std::endl;
    }
    virtual ~WidgetWithAllOperationsDeclared()
    {
        std::cout << "WidgetWithAllOperationsDeclared - inside destructor" << std::endl;
    }
};
WidgetWithAllDefaultOperations GetWidgetWithAllDefaultOperations(){return WidgetWithAllDefaultOperations();}
//WidgetWithCopyConstructorOnly GetWidgetWithCopyConstructorOnly(){return WidgetWithCopyConstructorOnly();}  /// error. There is no move constructor in class
WidgetWithMoveConstructorOnly GetWidgetWithMoveConstructorOnly(){return WidgetWithMoveConstructorOnly();}
WidgetWithAllOperationsDeclared GetWidgetWithAllOperationsDeclared(){return WidgetWithAllOperationsDeclared();}
void GiveMeCopyOfWidget(WidgetWithAllDefaultOperations){}
void GiveMeCopyOfWidget(WidgetWithCopyConstructorOnly){}
void GiveMeCopyOfWidget(WidgetWithMoveConstructorOnly){}
void GiveMeCopyOfWidget(WidgetWithAllOperationsDeclared){}
void MoveWidgetToMe(WidgetWithAllDefaultOperations&&){}
void MoveWidgetToMe(WidgetWithCopyConstructorOnly&&){}
void MoveWidgetToMe(WidgetWithMoveConstructorOnly&&){}
void MoveWidgetToMe(WidgetWithAllOperationsDeclared&&){}



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
    if(true);
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
        withNoExceptionFunction();
        compilerNotDetectableProgblem();
    }

/// Item 15: Use constexpr whenever possible
///
    if(true)
    {
        std::cout <<  std::endl << std::endl << std:: endl
                   << "Item 15: Use constexpr whenever possible"
                   << std::endl << std::endl;
        int sz;
        //constexpr auto arraySize = sz; /// error! this variable is not known during compilation time!
        //std::array<int,sz> data1; /// the same problem
        constexpr auto arraySizeGood = 10;
        std::array<int,arraySizeGood> arrayGood; /// all good. arraySizeGood is known during translation time.
        const auto wrongArraySize = sz;
        //std::array<int,wrongArraySize> wrongArray; /// error. const value can be unknown during compilation time.
        std::array<int,pow(arraySizeGood,3)> arraySizeWithConsexprFunction;
        //std::array<int,pow(wrongArraySize,3)> arraySizeWithConsexprFunctionNotKnownInCompileTime; /// error because constexpr function here
                                                                                                  /// not return known in compile time value
        constexpr Point point1(0.991, 0.9821); /// in C++14 objects can be consexpr

        constexpr Point point2(1.234,-1.141);
        constexpr auto mid = midPoint(point1,point2);
        std::cout << "midPoint = (" << mid.x() << "," << mid.y() << ")" << std::endl;
        constexpr auto reflrectedPoint1 = reflection(point1);
        std::cout << "reflrectedPoint1 = (" << reflrectedPoint1.x() << "," << reflrectedPoint1.y() << ")" << std::endl;
    }


/// Item 16: Make const member functions thread safe
///
    if(true)
    {
        std::cout <<  std::endl << std::endl << std:: endl
                   << "Item 16: Make const member functions thread safe "
                   << std::endl << std::endl;
        classWithCaching object1;
        object1.computeValue();
        object1.computeValue();
        object1.computeValue();
        /// demonstration of data race
        ///
        std::cout << std::endl << "thread unsafety demonstration" << std::endl;
        std::thread first(callFromThread12);
        std::thread second(callFromThread12);
        first.join();
        second.join();

        /// solution problem with mutex
        ///
        std::cout << std::endl << "thread SAFETY with mutex" << std::endl;
        std::thread third(callFromThread34);
        std::thread fourth(callFromThread34);
        third.join();
        fourth.join();

        /// how many times without atomic
        ///
        std::cout << std::endl << "how many times without atomic: " << std::endl;
        std::thread fifth(callFromThread5);
        std::thread sixth(callFromThread6);
        fifth.join();
        sixth.join();

        /// how many times without atomic
        ///
        std::cout << std::endl << "how many times WITH atomic: " << std::endl;
        std::thread seventh(callFromThread7);
        std::thread eightth(callFromThread8);
        seventh.join();
        eightth.join();   /// there is problem connected with two statements in functions callFromThread7 and callFromThread8
    }

/// Item 17: Understand special member functino generation
///
    if(true)
    {
        std::cout <<  std::endl << std::endl << std:: endl
                   << "Item 17: Understand special member functino generation"
                   << std::endl << std::endl;

        std::cout << std::endl << "Class with all special member functions = default" << std::endl;
        WidgetWithAllDefaultOperations widget1;
        GiveMeCopyOfWidget(widget1);
        MoveWidgetToMe(GetWidgetWithAllDefaultOperations());

        std::cout << std::endl << "Class with only copy ctors" << std::endl;
        WidgetWithCopyConstructorOnly widget2;
        GiveMeCopyOfWidget(widget2);

        std::cout << std::endl << "Class with only move ctors" << std::endl;
        WidgetWithMoveConstructorOnly&& widget3 = WidgetWithMoveConstructorOnly();
        //GiveMeCopyOfWidget(widget3); /// error. Copying is not possible. There is no generated copy ctors...
        MoveWidgetToMe(GetWidgetWithMoveConstructorOnly()); /// there is no call from move ctor. But moving takes place.

        std::cout << std::endl << "Class with all special member functions" << std::endl;
        WidgetWithAllOperationsDeclared widget4;
        GiveMeCopyOfWidget(widget4);
        MoveWidgetToMe(GetWidgetWithAllOperationsDeclared()); /// there is no call from move ctor. But moving takes place.
    }
    
    return a.exec();
}
