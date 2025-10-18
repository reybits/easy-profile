#pragma once

#include <string>

class Section final
{
public:
    Section(const char* title);
    ~Section();
};

const char* ToString(bool value);

template <typename T>
std::string ToString(const T& value)
{
    return std::to_string(value).c_str();
}
