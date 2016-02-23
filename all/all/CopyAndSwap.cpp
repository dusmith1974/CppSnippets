#include <algorithm> // std::copy
#include <cstddef> // std::size_t

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

private:
	std::size_t mSize;
	int* mArray;
};

#ifdef SF
int main()
{
	DoWork();
}
#endif
}  // namespace CopyAndSwap