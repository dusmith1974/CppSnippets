#include <algorithm>
#include <set>
#include <iostream>
#include <string>

namespace algos
{
void DoWork()
{
	std::set<int> s{ 3,6,2,1,8 };
	auto pos = std::find_if(s.cbegin(), s.cend(), [=](decltype(s)::value_type val) { return val > 5; });

	if (pos == s.cend())
		return;

	std::cout << "pos " << *pos;
}

#ifdef SF
int main()
{
	DoWork();
}
#endif
};  // algos