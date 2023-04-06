#pragma once

#include <memory>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Math {
    class SerpinskyCemetery;
}

class App {
public:
    GLFWwindow* _window;

    App(const App&) = delete;
    App& operator=(const App&) = delete;
    App(App&&) = delete;
    App& operator=(App&&) = delete;

    App(int width, int height, const char* path);
    ~App();

    void setFractal(const std::shared_ptr<Math::SerpinskyCemetery>& fractal) noexcept;

    void start() noexcept;
    void saveImage(const std::string& filePath) const noexcept;
private:
    void init(const char* path);

    const int WIDTH;
    const int HEIGHT;
    std::shared_ptr<Math::SerpinskyCemetery> _fractal;
};
