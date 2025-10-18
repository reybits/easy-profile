#pragma once

#include <print>

class Section final
{
public:
    Section(const char* title)
    {
        std::println("{}", title);
    }

    ~Section()
    {
        std::println();
    }
};
