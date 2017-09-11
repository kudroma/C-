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

    }

    return a.exec();
}

