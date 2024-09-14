#pragma once

#include <GLFW/glfw3.h>

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

static void gl_clear_error()
{
    while (glGetError() != GL_NO_ERROR)
        ;
}

static bool gl_log_call(const char* function, const char* file, int line)
{
    GLenum error = glGetError();

    while (error != GL_NO_ERROR)
    {
        std::cout << "----------------------------------------------------" << "\n";
        std::cout << "[OPENGL ERROR] (" << error << "): " << "\n"
                  << function << "\n"
                  << file << " : " << line << "\n";
        std::cout << "----------------------------------------------------" << "\n";

        return false;
    }

    return true;
}
