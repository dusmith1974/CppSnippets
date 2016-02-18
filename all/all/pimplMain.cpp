#include "pimpl98.h"
#include "pimpl11.h"

// Use copy and swap with pImpl see EffC++#25

int main()
{
    p98::Widget w;
    p11::Widget w11;

    w.DoWork();
    //w11.DoWork();
 
    return 0;
}
