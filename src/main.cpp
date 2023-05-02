#include <iostream>
#include <memory>

#include "Exception/Exception.h"
#include "Math/SerpinskyCemetery.h"
#include "Math/Geometry.h"
#include "App/App.h"

std::shared_ptr<App> app;

void keyCallback(GLFWwindow *pWindow, int key, int scancode, int action, int mode);

// 1 - 5
// 2 - 6
// 4 - 7
int main(int argc, char** argv) {
    try {
        app = std::make_shared<App>("MainWindow", 729, 729, argv[0]);
        auto fractal = std::make_shared<Math::SerpinskyCemetery>(Math::Polygon{{0., 0.},
                                                                               {0., 1.},
                                                                               {1., 1.},
                                                                               {1., 0.}});
        fractal->setWH(app->getWidth(), app->getHeight());
        app->setFractal(fractal);
        app->setKeyCallback(keyCallback);
        app->start();
    } catch (Exception::Exception& ex) {
        std::cout << ex.what() << std::endl;
    } catch (std::runtime_error& ex) {
        std::cout << ex.what() << std::endl;
    }
    glfwTerminate();
    return 0;
}

unsigned int inputUInt() {
    std::string str;
    std::cin >> str;
    while (true) {
        try {
            return std::stoi(str);
        } catch (const std::invalid_argument& ex) {
            std::cout << "Try again: ";
            std::cin >> str;
        }
    }
}

double inputDouble() {
    std::string str;
    std::cin >> str;
    while (true) {
        try {
            return std::stod(str);
        } catch (const std::invalid_argument& ex) {
            std::cout << "Try again: ";
            std::cin >> str;
        }
    }
}

std::pair<double, double> inputTwoDouble() {
    std::string str1, str2;
    std::cin >> str1 >> str2;
    while (true) {
        try {
            return {std::stod(str1), std::stod(str2)};
        } catch (const std::invalid_argument& ex) {
            std::cout << "Try again: ";
            std::cin >> str1 >> str2;
        }
    }
}

std::string inputImageName() {
    std::string name;
    std::cout << "Save image\n";
    std::cout << "Input file name: ";
    std::cin >> name;
    size_t found = name.find_last_of('.');
    if (found == std::string::npos or name.substr(found) != ".jpg") {
        name += ".jpg";
    }
    return name;
}

void changeLocation() {
    double left;
    double right;
    double bottom;
    double top;
    std::cout << "Change location\n";
    std::cout << "Input left and right: ";
    auto tmp = inputTwoDouble();
    left = tmp.first;
    right = tmp.second;
    std::cout << "Input bottom and top: ";
    tmp = inputTwoDouble();
    bottom = tmp.first;
    top = tmp.second;
    app->getFractal()->setViewPort({{bottom, left}, {top, right}});
    app->render();
}

void changeStep() {
    unsigned int new_step;
    std::cout << "Change amount step\n";
    std::cout << "Input amount step: ";
    new_step = inputUInt();
    app->getFractal()->setStep(new_step);
    app->render();
}

void changeRatio() {
    double newRatio;
    std::cout << "Change ratio\n";
    std::cout << "Input ratio: ";
    newRatio = inputDouble();
    app->getFractal()->setRatio(newRatio);
    app->render();
}

void changeInitPolygon() {
    Math::Polygon newPolygon;
    std::cout << "Change init polygon\n";
    std::cout << "Input point A: ";
    auto tmp = inputTwoDouble();
    newPolygon.a.x = tmp.first;
    newPolygon.a.y = tmp.second;

    std::cout << "Input point B: ";
    tmp = inputTwoDouble();
    newPolygon.b.x = tmp.first;
    newPolygon.b.y = tmp.second;

    std::cout << "Input point C: ";
    tmp = inputTwoDouble();
    newPolygon.c.x = tmp.first;
    newPolygon.c.y = tmp.second;

    std::cout << "Input point D: ";
    tmp = inputTwoDouble();
    newPolygon.d.x = tmp.first;
    newPolygon.d.y = tmp.second;

    app->getFractal()->setInitPolygon(newPolygon);
    app->render();
}

void save() {
    std::string name = inputImageName();
    app->saveImage(name);
}

void keyCallback(GLFWwindow *pWindow, int key, int scancode, int action, int mode) {
    if (       key == GLFW_KEY_M and action == GLFW_PRESS) {
        changeLocation();
    } else if (key == GLFW_KEY_I and action == GLFW_PRESS) {
        changeStep();
    } else if (key == GLFW_KEY_P and action == GLFW_PRESS) {
        changeInitPolygon();
    } else if (key == GLFW_KEY_R and action == GLFW_PRESS) {
        changeRatio();
    } else if (key == GLFW_KEY_S and action == GLFW_PRESS) {
        save();
    } else if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
            glfwSetWindowShouldClose(pWindow, GL_TRUE);
    }
}