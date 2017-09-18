#include "widget.h"
#include <vector>
#include <string>
#include <iostream>

class Widget::Impl
{
public:
    Impl(){std::cout << "Widget::Impl is created!" << std::endl;}
    std::string string{"dsfsdfs"};
    std::vector<int> vector{10};
    ~Impl(){std::cout << "Widget::Impl is destroyed!" << std::endl;}
};

Widget::Widget() : pImpl(std::make_unique<Impl>())
{
    std::cout << "Widget is created!" << std::endl;
}

Widget::~Widget()
{
    std::cout << "Widget is destroyed!" << std::endl;
}

Widget::Widget(Widget&&) = default;

Widget& Widget::operator=(Widget&&) = default;


Widget::Widget(Widget &rhs) : pImpl(std::make_unique<Impl>(*rhs.pImpl))
{
    std::cout << std::endl << "Widget is copied!" << std::endl << std::endl;
}

Widget& Widget::operator=(Widget &rhs)
{
    std::cout << "assign Widget!" << std::endl;
    *pImpl = *rhs.pImpl;
    return *this;
}
