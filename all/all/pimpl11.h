#pragma once

#include <memory>

namespace p11
{

class Widget
{
public:
    Widget();
    ~Widget();

    Widget(const Widget& rhs);
    Widget& operator=(const Widget& rhs);

    Widget(Widget&& rhs) noexcept;
    Widget& operator=(Widget&& rhs) noexcept;

private:
    struct Impl;
    std::unique_ptr<Impl> pImpl;
};

}  // namespace p11

