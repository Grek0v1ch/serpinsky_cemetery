#include "App.h"

#include <iostream>

#include <glm/gtc/matrix_transform.hpp>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../../external/stb_image_write.h"

#include "../Math/SerpinskyCemetery.h"
#include "../ResourceManager/ResourceManager.h"
#include "../Renderer/ShaderProgram.h"

App::App(const std::string& windowName, int width, int height, const std::string& path)
: _window(nullptr)
, WIDTH(width)
, HEIGHT(height)
, _fractal(nullptr) {
    ResourceManager::instance().setExecutablePath(path);
    init(windowName);
}

std::shared_ptr<Math::SerpinskyCemetery> App::getFractal() const noexcept {
    return _fractal;
}

void App::setFractal(const std::shared_ptr<Math::SerpinskyCemetery>& fractal) noexcept {
    _fractal = fractal;
}

void App::setKeyCallback(void (*func)(GLFWwindow *, int, int, int, int)) {
    glfwSetKeyCallback(_window, func);
}

void App::render() const noexcept {
    glClear(GL_COLOR_BUFFER_BIT);
    if (_fractal) {
        _fractal->render();
    }
    glfwSwapBuffers(_window);
}

void App::start() noexcept {
    while (! glfwWindowShouldClose(_window)) {
        render();
        glfwPollEvents();
    }
}

void App::saveImage(const std::string& filePath) const noexcept {
    int width, height;
    glfwGetFramebufferSize(_window, &width, &height);
    GLsizei nrChannels = 3;
    GLsizei stride = nrChannels * width;
    stride += (stride % 4) ? (4 - stride % 4) : 0;
    GLsizei bufferSize = stride * height;
    std::vector<char> buffer(bufferSize);
    glPixelStorei(GL_PACK_ALIGNMENT, 4);
    glReadBuffer(GL_FRONT);
    glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, buffer.data());
    stbi_flip_vertically_on_write(true);
    stbi_write_png(filePath.data(), width, height, nrChannels, buffer.data(), stride);
}

void App::init(const std::string& windowName) {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    _window = glfwCreateWindow(WIDTH, HEIGHT, windowName.data(), nullptr, nullptr);
    if (! _window) {
        throw std::runtime_error("glfwCreateWindow failed!");
    }
    glfwMakeContextCurrent(_window);
    if (! gladLoadGL()) {
        throw std::runtime_error("Can't load GLAD");
    }
    int width, height;
    glfwGetFramebufferSize(_window, &width, &height);
    glViewport(0, 0, width, height);
    glClearColor(66.0 / 255, 170.0 / 255, 255.0 / 255, 1.0f);
    glm::mat4 projectionMatrix = glm::ortho(0.0f, static_cast<float>(WIDTH),
                                            0.0f, static_cast<float>(HEIGHT),
                                            -100.0f, 100.0f);
    ResourceManager& resourceManager = ResourceManager::instance();
    auto shaderProgram = resourceManager.loadShaderProgram("SpriteShader",
                                                            "res/vertex_shader.txt",
                                                            "res/fragment_shader.txt");
    shaderProgram->setUniform("projectionMatrix", projectionMatrix);
}
