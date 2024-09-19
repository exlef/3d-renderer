#include "open_gl_error_checking.hpp"


#include <GL/glew.h>
#include <GLFW/glfw3.h>

void gl_clear_error()
{
    while (glGetError() != GL_NO_ERROR)
        ;
}

bool gl_log_call(const char* function, const char* file, int line)
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