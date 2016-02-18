#include "pimpl98.h"

#include "gadget.h"
#include <string>
#include <vector>

namespace p98
{

struct Widget::Impl
{
    std::string name;
    std::vector<double> data;
    Gadget g1, g2, g3;
};

Widget::Widget() : pImpl(new Impl)
{
}

Widget::~Widget()
{
    delete pImpl;
    pImpl = NULL;
}

Widget::DoWork()
{
    pImpl->name = "Boxer";
    pImpl->data = { 1.0, 2.0 };
    pImpl->g1;
}

}  // namespace p98
