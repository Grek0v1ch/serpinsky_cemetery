#include <iostream>
#include <memory>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../external/stb_image_write.h"

#include "Exception/Exception.h"
#include "Renderer/ShaderProgram.h"
#include "ResourceManager/ResourceManager.h"
#include "Math/Square.h"
#include "Math/TSquareFractal.h"
#include "App/App.h"

void glfwKeyCallback(GLFWwindow* pWindow, int key, int scancode, int action, int mode) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(pWindow, GL_TRUE);
    }
}

int main(int argc, char** argv) {
    std::cout << "Starting GLFW context, OpenGL 4.1" << std::endl;
    try {
        App app(800, 800, argv[0]);
        auto  fractal = std::make_shared<Math::TSquareFractal>(Math::TSquareFractal({0.0f, 0.0f},
                                     400.0,
                                     7,
                                     ResourceManager::instance().getShaderProgram("DefaultShader")));
        app.setFractal(fractal);
        app.start();

    } catch (Exception::Exception& ex) {
        std::cout << ex.what() << std::endl;
    }
    glfwTerminate();
    return 0;
}