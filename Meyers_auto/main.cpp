#include <QCoreApplication>
#include <iostream>
#include <deque>
#include <typeinfo>
#include <boost/type_index.hpp>
#include <QObject>
#include <functional>
#include <memory>
#include <QList>
#include <QTimer>

/// this test application is devoted to testing of new c++ feature
/// after Scott Meyers book
///
/// Chapter2: auto
///

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

std::function<bool(const std::unique_ptr<QObject>, const std::unique_ptr<QObject>)>func;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

/// Item 5: Prefer auto to explicit type declarations
///
    if(true)
    {
        //auto x2;  /// error because auto variable must be initialized
        std::deque<QObject*> dequeOfObjects;
        dequeOfObjects.push_back(new QObject);
        dequeOfObjects.push_back(new QObject);
        dequeOfObjects.push_back(new QObject);
        dequeOfObjects.push_back(new QTimer);
        for(const auto& object : dequeOfObjects)
        {
            static int i = 1;
            std::cout << i << " object class - " << object->metaObject()->className() << std::endl;
            i++;
        }
    }

/// Item 6: Use the explicitely typed initializer idiom when auto deduces undesired types
///
    if(true)
    {
        std::vector<bool> vector;
        vector.push_back(false);
        auto x1 = vector[0];
        std::cout << "vector<bool>[0] without casting is   " << x1 << std::endl;
        auto x2 = static_cast<bool>(vector[0]);
        std::cout << "static_cast<bool>(vector[0]) without casting is   " << x2 << std::endl;
    }

    return a.exec();
}
