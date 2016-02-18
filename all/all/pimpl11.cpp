#include "pimpl11.h"

#include <string>
#include <vector>

#include "gadget.h"

namespace p11
{

struct Widget::Impl
{
    std::string name;
    std::vector<double> data;
    Gadget g1, g2, g3;    
};

Widget::Widget() : pImpl(std::make_unique<Impl>())
{
}

Widget::~Widget() = default;

Widget::Widget(const Widget& rhs) : pImpl(nullptr)
{
    if (rhs.pImpl)
    {
        pImpl = std::make_unique<Impl>(*rhs.pImpl);
    }
}

Widget& Widget::operator=(const Widget& rhs)
{
    if (!rhs.pImpl)
    {
        pImpl.reset();
    }
    else if (!pImpl) 
    {
        pImpl = std::make_unique<Impl>(*rhs.pImpl);
    }
    else
    {
        *pImpl = *rhs.pImpl;
    }

    return *this;
}

Widget::Widget(Widget&& rhs) noexcept = default;
Widget& Widget::operator=(Widget&& rhs) noexcept = default;

}  // namespace p11
