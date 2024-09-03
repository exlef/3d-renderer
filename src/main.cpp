#include "ex3d/renderer.hpp"

int main()
{
    ex::renderer renderer;
    renderer.create_window(800, 600, "test");
    std::cout << "hello" << std::endl;
}