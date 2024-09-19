#pragma once


#include <iostream>

namespace ex
{

} // namespace ex

#define ASSERT(x) \
    if (!(x))     \
        __builtin_debugtrap();

#define glc(x)        \
    gl_clear_error(); \
    x;                \
    ASSERT(gl_log_call(#x, __FILE__, __LINE__))

void gl_clear_error();

bool gl_log_call(const char* function, const char* file, int line);
