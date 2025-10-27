#include "helpers.h"

#include <cstdio>

Section::Section(const char* title)
{
    ::printf("%s\n", title);
}

Section::~Section()
{
    ::printf("\n");
}

const char* ToString(bool value)
{
    return value ? "true" : "false";
}
