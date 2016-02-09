#pragma once



class A
{
public:
	A();
	explicit A(int iID);
	A(int iID, const std::string& name);

	A(const A& other);
	A& operator=(const A other)

};
