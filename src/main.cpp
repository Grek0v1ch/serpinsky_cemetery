#include <iostream>
#include <memory>

#include "Exception/Exception.h"
#include "Math/SerpinskyCemetery.h"
#include "App/App.h"

std::shared_ptr<App> app;

std::pair<float, float> input_2float();
unsigned int input_uint();
void change_location();
void change_step();
void keyCallback(GLFWwindow *pWindow, int key, int scancode, int action, int mode);

// 243 - 5
// 729 - 6
// 2187 - 7
// 6561 - 8
int main(int argc, char** argv) {
    try {
        app = std::make_shared<App>("MainWindow", 729, 729, argv[0]);
        auto fractal = std::make_shared<Math::SerpinskyCemetery>(Math::Polygon{{0., 0.},
                                                                               {0., 0.5},
                                                                               {1., 1.},
                                                                               {1., 0.}},
                                                                 2);
        fractal->setWH(app->getWidth(), app->getHeight());
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

std::pair<float, float> input_2float() {
    while (true) {
        try {
            std::string str1, str2;
            std::cin >> str1 >> str2;
            return {static_cast<float>(std::stod(str1)),
                    static_cast<float>(std::stod(str2))};
        } catch (const std::invalid_argument& ex) {
            std::cout << "Try again: ";
        }
    }
}

unsigned int input_uint() {
    std::string str;
    std::cin >> str;
    while (true) {
        try {
            return std::stoi(str);
        } catch (const std::invalid_argument& ex) {
            std::cout << "Try again: ";
        }
    }
}

void change_location() {
    float left;
    float right;
    float bottom;
    float top;
    std::cout << "Change location\n";
    std::cout << "Input left and right: ";
    auto tmp = input_2float();
    left = tmp.first;
    right = tmp.second;
    std::cout << "Input bottom and top: ";
    tmp = input_2float();
    bottom = tmp.first;
    top = tmp.second;
    app->setLocation(left, right, bottom, top);
    app->render();
}

void change_step() {
    unsigned int new_step;
    std::cout << "Change amount step\n";
    std::cout << "Input amount step: ";
    new_step = input_uint();
    auto fractal = app->getFractal();
    fractal->setStep(new_step);
    app->render();
}

void keyCallback(GLFWwindow *pWindow, int key, int scancode, int action, int mode) {
    if (key == GLFW_KEY_M and action == GLFW_PRESS) {
        change_location();
    } else if (key == GLFW_KEY_S and action == GLFW_PRESS) {
        change_step();
    } else if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
            glfwSetWindowShouldClose(pWindow, GL_TRUE);
    }
}