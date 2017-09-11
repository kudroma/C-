#include <QCoreApplication>
#include <iostream>
#include <deque>
#include <typeinfo>
#include <boost/type_index.hpp>

/// this test application is devoted to testing of new c++ feature
/// after Scott Meyers book
///

/// use debugger in order to see type of param inside function
///
template<typename T> void f(T& param);
template<typename T> void f1(const T& param);
template<typename T> void f2(T&& param);
template<typename T> void f3(T param);
template<typename T> void f4(T* param);

/// return size of array
///
template<typename T, std::size_t N>
constexpr std::size_t arraySize(T(&)[N])
{
    return N;
}

void sumFunc(int param1, double param2)
{
    param1+param2;
}

template<typename T> void fList(std::initializer_list<T> param);

//auto createInitList()
//{
//    return {1, 2, 3};
//}

template<typename Container, typename Index>
auto authAndAccess(Container& c, Index i)
->decltype(c[i])
{
    return c[i];
}

template<typename Container, typename Index>
auto authAndAccess1(Container& c, Index i)
{
    return c[i];
}

template<typename Container, typename Index>
decltype(auto)
authAndAccess2(Container&& c, Index i)
{
    return c[i];
}

template<typename Container, typename Index>
decltype(auto)
getX()
{
    int x = 0;
    return (x); /// !!!!!!!!!!! reference to local variable (int&) !!!!!!!!!!!
}

template<typename T> class tD;

template<typename T>
void showTypeBoost(const T& param)
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

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

/// Item 1
///
    if(true)
    {
        int x = 27;
        const int cx = 27;
        const int& rx = 27;
        f(x);
        f(cx);
        f(rx);

        f1(x);
        f1(cx);
        f1(rx);

        f2(x);
        f2(cx);
        f2(rx);
        f2(27);

        const char* name = "My name is Roman";
        f(name);
        f4(name);

        int keyVals[] = { 1, 3, 7, 9, 11, 22, 35 };
        std::cout << "size of array is: " << arraySize(keyVals) << "\n";
        std::array<int, arraySize(keyVals)> mappedVals;

        /// function as param
        ///
        f(sumFunc);
        f4(sumFunc);
    }

/// Item 2: Understand auto type deduction
///
    if(true)
    {
        auto x = 27;
        const auto cx = x;
        const auto& rx = cx;

        auto&& uref1 = x; /// lvalue
        auto&& uref2 = rx; /// lvalue
        auto&& uref3 = 27; /// rvalue

        const char name[] = // name's type is const char[13]
        "R. N. Briggs";
        auto arr1 = name; /// const char*
        auto& arr2 = name; /// const char[13]
        auto func1 = sumFunc; /// void(*)(int, double)
        auto& func2 = sumFunc; /// void(&)(int,double)

        auto x1 = 27; /// int
        auto x2(27);  /// int
        auto x3{ 27 }; /// std::initializer_list<int>
        auto x4 = { 27 };  /// std::initializer_list<int>
        // auto x5 = {1, 2, 3.0};
        auto x6 = { 11, 23, 9 };
        fList({1, 2, 3});

        /// show type using compiler
        ///
//        std::cout << typeid(x).name() << std::endl;
//        std::cout << typeid(cx).name() << std::endl;
//        std::cout << typeid(rx).name() << std::endl;
//        std::cout << typeid(uref1).name() << std::endl;
//        std::cout << typeid(uref2).name() << std::endl;
//        std::cout << typeid(uref3).name() << std::endl;
//        std::cout << typeid(arr1).name() << std::endl;
//        std::cout << typeid(arr2).name() << std::endl;
//        std::cout << typeid(func1).name() << std::endl;
//        std::cout << typeid(func2).name() << std::endl;
//        std::cout << typeid(x1).name() << std::endl;
//        std::cout << typeid(x2).name() << std::endl;
//        std::cout << typeid(x3).name() << std::endl;
//        std::cout << typeid(x4).name() << std::endl;
//        std::cout << typeid(x6).name() << std::endl;

        /// show type using boost functionality
        ///
//        showTypeBoost(x);
//        showTypeBoost(cx);
//        showTypeBoost(rx);
//        showTypeBoost(uref1);
//        showTypeBoost(uref2);
//        showTypeBoost(uref3);
//        showTypeBoost(arr1);
//        showTypeBoost(arr2);
//        showTypeBoost(func1);
//        showTypeBoost(func2);
//        showTypeBoost(x1);
//        showTypeBoost(x2);
//        showTypeBoost(x3);
//        showTypeBoost(x4);
//        showTypeBoost(x6);

        /// show type using compiler error message
        ///
        //tD<decltype(func1)> xType;  /// special error to see deduced type of variable
    }

/// Item 3: Understand decltype
///
    if(true)
    {
        std::deque<int> d;
        d.push_back(5);
        d.push_back(5);
        d.push_back(5);
        d.push_back(5);
        d.push_back(5);
        authAndAccess1(d,4);
        //authAndAccess1(d,4) = 10;
        authAndAccess2(d,4) = 10;
        for(int i = 0; i < d.size(); i++)
        {
            std::cout << d.at(i) << std::endl;
        }
    }

/// Item 4: Know how to view deduced types
///


    return a.exec();
}

template<typename T> void f(T &param)
{
    std::cout << "inside function template<typename T> void f(T& param)\n";
}

template<typename T> void f1(const T &param)
{
    std::cout << "inside function template<typename T> void f1(const T& param)\n";
}

template<typename T> void f2(T&& param)
{
    std::cout << "inside function template<typename T> void f2(T&& param)\n";
}

template<typename T> void f3(T param)
{
    std::cout << "inside function template<typename T> void f3(T param)\n";
}

template<typename T> void f4(T* param)
{
    std::cout << "inside function template<typename T> void f4(T* param)\n";
}

template<typename T> void fList(std::initializer_list<T> param)
{
    std::cout << "template<typename T> vois fList(std::initializer_list<T> param)\n";
}
