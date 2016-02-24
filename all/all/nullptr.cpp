#include <cstddef>
#include <cstdint>

namespace np
{
void fn(int n)
{
	(void)n;
}

void fn(int* n)
{
	(void)n;
}

void fn(void* n)
{
	(void)n;
}

void fn(nullptr_t n)
{
	(void)n;
}

void fn(intptr_t* t)
{
	(void)t;
}

void DoWork()
{
	int n = 5;
	int* pn = &n;

	// will call int* overload (if exists)
	int* pn2;
	int* pn3 = NULL;
	int* pn4 = nullptr;
	int* pn5{};

	// will call void* overload
	void* pn6{};

	fn(pn);
	fn(pn2);
	fn(pn3);
	fn(pn4);
	fn(pn5);
	fn(pn6);

	// both call fn(int) NULL is just 0
	fn(0);
	fn(NULL);

	intptr_t* t = 0;
	fn(t);

	long x = 34L;
	fn(static_cast<intptr_t>(x));
	fn(reinterpret_cast<intptr_t*>(x));

	fn(static_cast<int>(NULL)); // cast req'd for gcc (which has internal NULL type)

	// Would call nullptr_t, void* or int* (in that order)
	// nullptr_t is convertible to any pointer, but not to any integral value
	fn(nullptr);
}

#ifdef SF
int main(void) { DoWork(); }
#endif
}  // namespace np