#include "pimpl98.h"
#include "pimpl11.h"

// Use copy and swap with pImpl see EffC++#25,29

namespace pimpl
{
void DoWork()
{
	p98::Widget w;
	p11::Widget w11;

	w.DoWork();
	//w11.DoWork();
}

#ifdef SF
int main() { DoWork(); }
#endif
}  // namespace pimpl