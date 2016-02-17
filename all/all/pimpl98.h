#pragma once

class Widget
{
public:
    Widget();
    ~Widget();

private:
    struct Impl;
    struct Impl* pImpl;
};
