#pragma once
#include <memory>

class Widget
{
public:
    /// constructor
    ///
    Widget();
    /// move special member functions
    ///
    Widget(Widget&&);
    Widget& operator=(Widget&&);
    /// copy special member functions
    ///
    Widget(Widget&);
    Widget& operator=(Widget&);
    /// destructor
    ///
    ~Widget();

private:
    class Impl;
    std::unique_ptr<Impl> pImpl;
};
