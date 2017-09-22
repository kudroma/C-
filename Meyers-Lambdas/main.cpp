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

/// Item 31: Avoid default capture modes
///
/// class members are not captured from lambdas in member functions
class wrongLambda
{
public:
    void func()
    {
        //auto lambda = [divisor](){};  /// error. Non-local variable can't be captured by lambda.
    }
    void func2()
    {
        auto lambda = [divisor = divisor](){std::cout << "inside lambda of func2!" << std::endl;}; /// generalized lambda capture of C++14
        lambda();
    }

private:
    int divisor;
};
/// Item 33: Use decltype to auto&& parameters to std::forward them
///
template<typename... Types>
void funcT(Types&&... params){std::cout << "inside function funcT" << std::endl;}




int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

/// Item 31: Avoid default capture modes
///
    if(true)
    {
        std::cout <<  std::endl << std::endl << std:: endl
                   << "Item 31: Avoid default capture modes"
                   << std::endl << std::endl;
        auto lambda1 = [](int y){return y;};
        auto lambda2 = lambda1; /// copy of closure object
        std::cout << "first lambda returns: " << lambda1(5) << " ans second - it's copy - " << lambda2(5) << std::endl;

        using FilterContainer = std::vector<std::function<bool(int)> >;
        FilterContainer filters;
        if(true)
        {
            Investment* divisor = new Investment();
            filters.emplace_back([&](float value){return value / divisor->x() == 0;});  /// default by-reference capture mode
            filters.emplace_back([&divisor](float value){return value / divisor->x() == 0;});
            filters.emplace_back([=](float value){return value / divisor->x() == 0;}); /// default by-value capture mode
            divisor = nullptr;
        }
        /// undefinde behavior in the following two rows because of dangling pointer
        ///
        //std::cout << "lambda call with local variable capture: " << filters[0](10) << std::endl;
        //std::cout << "lambda call with local variable reference capture: " << filters[1](10) << std::endl;
        /// it's also, because only copy of pointer is get by value
        /// but it works.
        /// why???
        ///
        std::cout << "lambda call with local variable by value capture: " << filters[2](10) << std::endl;
        wrongLambda object;
        object.func2();
    }

/// Item 32: Use init capture to move objects into closures
///
    if(true)
    {
        std::cout <<  std::endl << std::endl << std:: endl
                   << "Item 32: Use init capture to move objects into closures"
                   << std::endl << std::endl;

        auto inv = std::make_unique<Investment>();
        //auto lambdaWrong = [inv](){std::cout << inv->x() << std::endl;}; /// error, because unique_ptr is move-only type.
        auto lambdaGood = [inv = std::move(inv)](){std::cout << "Init capture is good - therefore get " << inv->x() << std::endl;};
        lambdaGood();
        auto func = [inv = std::make_unique<Investment>()]() {std::cout << "init capture is good! " << inv->x() << std::endl;};
        func();
    }


/// Item 33: Use decltype to auto&& parameters to std::forward them
///
    if(true)
    {
        std::cout <<  std::endl << std::endl << std:: endl
                   << "Item 33: Use decltype to auto&& parameters to std::forward them"
                   << std::endl << std::endl;
        /// lambda with perfect forwarding
        ///
        auto f = [](auto&& param)
        {
            funcT(std::forward<decltype(param)>(param));
        };
        /// variadic lambdas
        ///
        auto fVar = [](auto&&... params)
        {
            funcT(std::forward<decltype(params)>(params)...);
        };
        fVar(1,2,3,"dfsfsdfs",Investment(),std::move(Investment()));
    }

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
