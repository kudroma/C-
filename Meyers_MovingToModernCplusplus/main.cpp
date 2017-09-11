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
/// Chapter3: Moving to Modern C++
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

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

/// Item 7:
///
    if(true)
    {

    }

    return a.exec();
}
