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
#include <widget.h>

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

/// Item 18: Use std::unique_ptr for exclusive-ownership resource management
///
class Investment
{
public:
    Investment(){std::cout << "Create Investment" << std::endl;}
    virtual std::string type()const
    {
        return m_type;
    }
    virtual ~Investment() {std::cout << "Delete Investment" << std::endl;}
    Investment(Investment&&) = default;
protected:
        std::string m_type {"Investment"};
};
class Bond : public Investment
{
public:
    Bond(){std::cout << "Create Bond" << std::endl; m_type = std::string("Bond");}
    virtual std::string type()const
    {
        return m_type;
    }
    virtual ~Bond() {std::cout << "Delete Bond" << std::endl;}
    Bond(Bond&&) = default;
};
class Stock : public Investment
{
public:
    Stock(){std::cout << "Create Stock" << std::endl; m_type = std::string("Stock");}
    virtual std::string type()const
    {
        return m_type;
    }
    virtual ~Stock() {std::cout << "Delete Stock" << std::endl;}
    Stock(Stock&&) = default;
};
/// now caller of factory method is not responsible for deleting of objects created
///
std::unique_ptr<Investment> createInvestment(std::string type = "")
{
    if(type == std::string("Bond"))
    {
        return std::unique_ptr<Investment>(new Bond);
    }
    else if(type == std::string("Stock"))
    {
        return std::unique_ptr<Investment>(new Stock);
    }
    else
    {
        return std::unique_ptr<Investment>(new Investment);
    }
}

auto createInvestmentWithOwnDeleter(std::string type = "")
{
    auto deleter = [](Investment *pInvestment)
                    {
                        std::cout << "inside deleter" << std::endl;
                        delete pInvestment;
                    };
    std::unique_ptr<Investment,decltype(deleter)> pInv(nullptr,deleter);
    if(type == std::string("Bond"))
    {
        pInv.reset(new Bond);
    }
    else if(type == std::string("Stock"))
    {
        pInv.reset(new Stock);
    }
    else
    {
        pInv.reset(new Investment);
    }
    return pInv;
}

/// Item 20: Use std::weak_ptr for std::shared_ptr like pointers that can dangle
///
std::shared_ptr<Investment> fastLoadInvestment()
{
    static std::weak_ptr<Investment> wInv;
    static int i = 0;
    if(!i)
    {
        /// first call to function!
        ///
        std::shared_ptr<Investment> inv;
        std::cout << "creation of cache!" << std::endl;
        inv = std::shared_ptr<Investment>(new Investment);
        wInv = inv;
        i++;
        return inv;
    }
    else
    {
        auto invShp = wInv.lock();
        if(invShp)
        {
            std::cout << "object is not dangled!" << std::endl;
        }
        else
        {
            std::cout << "object is dangled!" << std::endl;
        }
        i++;
        return invShp;
    }
}

/// Item 21: Prefer std::make_unique and std::make_shared to direct use of new
///
void processInvestment(std::shared_ptr<Investment> inv, int priority)
{
    std::cout << "function is called!" << std::endl;
}
int getPriority()
{
    throw 1;
    return 1;
}


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

/// Item 18: Use std::unique_ptr for exclusive-ownership resource management
///
    if(true)
    {
        std::cout <<  std::endl << std::endl << std:: endl
                   << "Item 18: Use std::unique_ptr for exclusive-ownership resource management"
                   << std::endl << std::endl;
        auto invest1 = createInvestment();
        auto invest2 = createInvestment("Bond");
        auto invest3 = createInvestment("Stock"); /// all constructors and destructors are called
        //invest3 = invest2; /// error. unique_ptr must be unique!
        std::cout << std::endl << "Create unique_ptr with custom deleter" << std::endl;
        auto invest4 = createInvestmentWithOwnDeleter();
        auto invest5 = createInvestmentWithOwnDeleter("Bond");
        auto invest6 = createInvestmentWithOwnDeleter("Stock"); /// all constructors and destructors are called
        std::shared_ptr<Investment> investShp = createInvestment(); /// shared_ptr can be easily converted from unique_ptr
    }

/// Item 19: Use std::shared_ptr for shared-ownership resource management
///
    if(true)
    {
        std::cout <<  std::endl << std::endl << std:: endl
                   << "Item 19: Use std::shared_ptr for shared-ownership resource management"
                   << std::endl << std::endl;
        constexpr int number = 10;
        std::vector<std::shared_ptr<Investment> >  vector(number);
        auto customDeleter = [](Investment* ptr)
                            {
                                std::cout << "delete object which is shared" << std::endl;
                                delete ptr;
                            };
        vector[0] = std::shared_ptr<Investment>(new Investment, customDeleter);
        std::cout << "size of shared pointer with custom deleter: " << sizeof(vector[0]) << std::endl;
        std::cout << "size of shared pointer without custom deleter: " << sizeof(std::shared_ptr<Investment>(new Investment)) << std::endl;
        for(int i = 1; i < number; i++)
        {
            vector[i] = vector[0];
            std::cout << "number of shared pointers: " << vector[0].use_count() << std::endl;
        }
    }

/// Item 20: Use std::weak_ptr for std::shared_ptr like pointers that can dangle
///
    if(true)
    {
        std::cout <<  std::endl << std::endl << std:: endl
                   << "Item 20: Use std::weak_ptr for std::shared_ptr like pointers that can dangle"
                   << std::endl << std::endl;

        auto invShp = std::make_shared<Investment>();
        auto weakInvPtr = std::weak_ptr<Investment>(invShp); /// create weak pointer from shared pointer
        std::cout << "if weak pointer is expired: " << weakInvPtr.expired() << std::endl;
        invShp = nullptr; /// Investment is destroyed and weakInvPtr now dangles
        std::cout << "after deletion of Investment" << std::endl;
        std::cout << "if weak pointer is expired: " << weakInvPtr.expired() << std::endl;
        auto possibleShp = weakInvPtr.lock(); /// create shared pointer from weak in a thread safety manner
        if(possibleShp == nullptr)
        {
            std::cout << "locked pointer is nullptr!" << std::endl;
        }
        try{
            std::shared_ptr<Investment> possibleShp(weakInvPtr); /// second way to allocate shared ptr from weak one. If weak is expired there is exception thrown.
        }
        catch(std::bad_weak_ptr)
        {
            std::cout << "exception! You've tried to allocate bad weak pointer!" << std::endl;
        }
        /// factory method with caching
        ///
        constexpr int number = 10;
        std::vector<std::shared_ptr<Investment> > invs{number};
        for( int i = 0; i < number; i++)
        {
            invs[i] = fastLoadInvestment();
        }
        /// clean invs
        ///
        for(int i = 0; i < number; i++)
        {
            std::cout << "reference count is - " << invs[i].use_count() << std::endl;
            invs[i] = nullptr;
        }
        possibleShp = fastLoadInvestment();
        if(!possibleShp)
        {
            std::cout << "now factory can't produced Investment because cache is clear!" << std::endl;
        }
    }

/// Item 21: Prefer std::make_unique and std::make_shared to direct use of new
///
    if(true)
    {
        std::cout <<  std::endl << std::endl << std:: endl
                   << "Item 21: Prefer std::make_unique and std::make_shared to direct use of new"
                   << std::endl << std::endl;

        std::shared_ptr<Investment> inv1 = std::make_shared<Investment>();
        std::unique_ptr<Investment> inv2 = std::make_unique<Investment>(); /// this won't compile in C++11
        auto inv3(std::make_shared<Investment>());
        /// demonstration of memory leaks
        ///
        processInvestment(std::make_shared<Investment>(),2);
        for(int i = 0; i < 5; i++)
        {
            try{
                processInvestment((std::shared_ptr<Investment>(new Investment())),getPriority());
            }
            catch(int a)
            {
                std::cout << "exception is catched and memory leak is possible !" << std::endl;
            };
        }
        /// there is no memory leaks
        ///
        for(int i = 0; i < 5; i++)
        {
            try{
                processInvestment(std::make_shared<Investment>(),getPriority());
            }
            catch(int a)
            {
                std::cout << "exception is catched but memory leak is impossible !" << std::endl;
            };
        }
        /// another exception safe way with effective performance
        ///
        for(int i = 0; i < 5; i++)
        {
            try{
                auto cusDel = [](Investment* inv){delete inv;};
                std::shared_ptr<Investment> inv(new Investment, cusDel);
                processInvestment(std::move(inv),getPriority());
            }
            catch(int a)
            {
                std::cout << "exception is catched but memory leak is impossible !" << std::endl;
            };
        }
    }

/// Item 22: When using the Pimpl idiom, define special member functions int the implementation file
///
    if(true)
    {
        std::cout <<  std::endl << std::endl << std:: endl
                   << "Item 22: When using the Pimpl idiom, define special member "
                   << "functions int the implementation file"
                   << std::endl << std::endl;
        auto w1 = std::make_shared<Widget>();
        w1 = nullptr;
        auto lambda = [](Widget&&){std::cout << "moving Widget!" << std::endl;};
        lambda(Widget());
        Widget w2;
        Widget w3 = w2; /// there is call of copy ctor not operator=. I don't know why...
    }

    return a.exec();
}
