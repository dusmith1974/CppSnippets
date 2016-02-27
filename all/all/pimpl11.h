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

	friend void swap(Widget& first, Widget& second)
	{
		using std::swap;
		swap(first.pImpl, second.pImpl);
	}

private:
	struct Impl;
	std::unique_ptr<Impl> pImpl;
};
}  // namespace p11
