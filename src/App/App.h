#pragma once

#include <memory>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Math {
    class TSquareFractal;
}

class App {
public:
    App(const App&) = delete;
    App& operator=(const App&) = delete;
    App(App&&) = delete;
    App& operator=(App&&) = delete;

    App(int width, int height, const char* path);
    ~App();

    void setFractal(const std::shared_ptr<Math::TSquareFractal>& fractal) noexcept;

    void start() noexcept;
    void saveImage(const std::string& filePath) const noexcept;
private:
    void init(const char* path);

    const int WIDTH;
    const int HEIGHT;
    GLFWwindow* _window;
    std::shared_ptr<Math::TSquareFractal> _fractal;
};
