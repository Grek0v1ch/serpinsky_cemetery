#include <iostream>
#include <memory>

#include "Exception/Exception.h"
#include "Renderer/ShaderProgram.h"
#include "Math/SerpinskyCemetery.h"
#include "App/App.h"

std::shared_ptr<App> app;

void keyCallback(GLFWwindow *pWindow, int key, int scancode, int action, int mode) {
    if (key == GLFW_KEY_I and action == GLFW_PRESS) {
        float left;
        float right;
        float bottom;
        float top;
        std::cout << "Change location\n";
        std::cout << "Input left: ";
        std::cin >> left;
        std::cout << "Input right: ";
        std::cin >> right;
        std::cout << "Input bottom: ";
        std::cin >> bottom;
        std::cout << "Input top: ";
        std::cin >> top;
        app->setLocation(left, right, bottom, top);
        app->render();
    }
}

int main(int argc, char** argv) {
    try {
        app = std::make_shared<App>("MainWindow", 400, 400, argv[0]);
        auto fractal = std::make_shared<Math::SerpinskyCemetery>( // 6561
                Math::SerpinskyCemetery({0, 0}, {729, 729}, 6, "DefaultShader"));
        app->setFractal(fractal);
        app->setKeyCallback(keyCallback);
        app->start();
        app->saveImage("img1.jpg");
    } catch (Exception::Exception& ex) {
        std::cout << ex.what() << std::endl;
    } catch (std::runtime_error& ex) {
        std::cout << ex.what() << std::endl;
    }
    glfwTerminate();
    return 0;
}