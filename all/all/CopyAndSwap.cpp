#include <algorithm> // std::copy
#include <cstddef> // std::size_t

#include "CopyAndSwap.h"

// http://stackoverflow.com/questions/3279543/what-is-the-copy-and-swap-idiom

namespace CopyAndSwap
{
class dumb_array
{
public:
	// (default) constructor
	dumb_array(std::size_t size = 0)
		: mSize(size),
		mArray(mSize ? new int[mSize]() : 0)
	{
	}

	// copy-constructor
	dumb_array(const dumb_array& other)
		: mSize(other.mSize),
		mArray(mSize ? new int[mSize] : 0)
	{
		// note that this is non-throwing, because of the data
		// types being used; more attention to detail with regards
		// to exceptions must be given in a more general case, however
		std::copy(other.mArray, other.mArray + mSize, mArray);
	}

	// destructor
	~dumb_array()
	{
		delete[] mArray;
	}

	friend void swap(dumb_array& first, dumb_array& second) // nothrow
	{
		// enable ADL (not necessary in our case, but good practice)
		using std::swap;

		// by swapping the members of two classes,
		// the two classes are effectively swapped
		swap(first.mSize, second.mSize);
		swap(first.mArray, second.mArray);
	}

	dumb_array& operator=(dumb_array other)
	{
		swap(*this, other);
		return *this;
	}

private:
	std::size_t mSize;
	int* mArray;
};

void DoWork()
{
	dumb_array A(5);
	dumb_array B;
	B = A;
}

#ifdef SF
int main()
{
	DoWork();
}
#endif
}  // namespace CopyAndSwap