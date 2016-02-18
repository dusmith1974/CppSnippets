#pragma once

class Widget
{
public:
    Widget();
    ~Widget();

    DoWork();

private:
    struct Impl;
    struct Impl* pImpl;
};
