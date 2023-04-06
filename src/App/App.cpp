#include "App.h"

#include <iostream>

#include <glm/gtc/matrix_transform.hpp>

#include "../Math/SerpinskyCemetery.h"
#include "../ResourceManager/ResourceManager.h"
#include "../Renderer/ShaderProgram.h"
#include "../../external/stb_image_write.h"

App::App(int width, int height, const char* path) :
    WIDTH(width),
    HEIGHT(height),
    _window(nullptr),
    _fractal(nullptr) {
    init(path);
}

App::~App() {
    glfwTerminate();
}

void App::setFractal(const std::shared_ptr<Math::SerpinskyCemetery>& fractal) noexcept {
    _fractal = fractal;
}

void App::start() noexcept {
    glClear(GL_COLOR_BUFFER_BIT);
    if (_fractal) {
        _fractal->render();
    }
    glfwSwapBuffers(_window);
    saveImage("img.jpg");
    while (! glfwWindowShouldClose(_window)) {
        glfwPollEvents();
    }
}

void App::saveImage(const std::string& filePath) const noexcept {
    int* buffer = new int[ WIDTH * HEIGHT * 3 ];
    glPixelStorei(GL_PACK_ALIGNMENT, 4);
    glReadBuffer(GL_FRONT);
    glReadPixels(0, 0, WIDTH, HEIGHT, GL_RGB, GL_UNSIGNED_BYTE, buffer);
    stbi_flip_vertically_on_write(true);
    stbi_write_png(filePath.data(), WIDTH, HEIGHT, 3, buffer, 0);
    delete[] buffer;
}

void App::init(const char* path) {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    _window = glfwCreateWindow(WIDTH, HEIGHT, "MainWindow", nullptr, nullptr);
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
    glm::mat4 modelMatrix(1.0f);
    // modelMatrix = glm::translate(modelMatrix, glm::vec3(100.0f, 0.0f, 0.0f));
    ResourceManager& resourceManager = ResourceManager::instance();
    resourceManager.setExecutablePath(path);
    auto shader_program = resourceManager.loadShaderProgram("DefaultShader",
                                                            "res/vertex_shader.txt",
                                                            "res/fragment_shader.txt");
    shader_program->setUniform("projectionMatrix", projectionMatrix);
    shader_program->setUniform("modelMatrix", modelMatrix);
}
