#include <GLFW/glfw3.h>

int main(int argc, char** argv)
{
    if (!glfwInit()) {
        return 1;
    }

    GLFWwindow* window =
        glfwCreateWindow(640, 480, "Hello, world!", nullptr, nullptr);
    if (!window) {
        return 1;
    }

    glfwMakeContextCurrent(window);

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();

    return 0;
}
