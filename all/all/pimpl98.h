#pragma once

namespace p98
{
class Widget
{
public:
	Widget();
	~Widget();

	// TODO(smitdu)
	// add cctor and casop (based on c11)

	void DoWork();

private:
	struct Impl;
	struct Impl* pImpl;
};
}  // namespace p98
