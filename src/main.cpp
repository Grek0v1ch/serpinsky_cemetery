#include <iostream>
#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/vec2.hpp>
#include <glm/gtc/matrix_transform.hpp>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../external/stb_image_write.h"

#include "Exception/Exception.h"
#include "Renderer/ShaderProgram.h"
#include "ResourceManager/ResourceManager.h"
#include "Math/Square.h"
#include "Math/TSquareFractal.h"

const GLuint WIDTH = 800, HEIGHT = 800;

void glfwKeyCallback(GLFWwindow* pWindow, int key, int scancode, int action, int mode) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(pWindow, GL_TRUE);
    }
}

int main(int argc, char** argv) {
    std::cout << "Starting GLFW context, OpenGL 4.1" << std::endl;
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL", nullptr, nullptr);
    if (! window) {
        std::cout << "glfwCreateWindow failed!" << std::endl;
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, glfwKeyCallback);

    if (! gladLoadGL()) {
        std::cout << "Can't load GLAD" << std::endl;
        glfwTerminate();
        return -1;
    }
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);

    glClearColor(66.0 / 255, 170.0 / 255, 255.0 / 255, 1.0f);

    glm::mat4 projectionMatrix = glm::ortho(0.0f, static_cast<float>(WIDTH),
                                            0.0f, static_cast<float>(HEIGHT),
                                            -100.0f, 100.0f);
    glm::mat4 modelMatrix(1.0f);
    modelMatrix = glm::translate(modelMatrix, glm::vec3(400.0f, 400.0f, 0.0f));
    try {
        ResourceManager& resourceManager = ResourceManager::instance();
        resourceManager.setExecutablePath(argv[0]);
        auto shader_program = resourceManager.loadShaderProgram("DefaultShader",
                                                                "res/vertex_shader.txt",
                                                                "res/fragment_shader.txt");
        shader_program->setUniform("projectionMatrix", projectionMatrix);
        shader_program->setUniform("modelMatrix", modelMatrix);

        Math::TSquareFractal fractal({0.0f, 0.0f},
                                     static_cast<double>(WIDTH) / 2,
                                     7,
                                     shader_program);

        glClear(GL_COLOR_BUFFER_BIT);
        fractal.render();
        glfwSwapBuffers(window);
        int* buffer = new int[ WIDTH * HEIGHT * 3 ];
        glPixelStorei(GL_PACK_ALIGNMENT, 4);
        glReadBuffer(GL_FRONT);
        glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, buffer);
        stbi_flip_vertically_on_write(true);
        stbi_write_png("img.jpg", width, height, 3, buffer, 0);
        delete[] buffer;

        while (!glfwWindowShouldClose(window)) {
            glfwPollEvents();
        }
    } catch (Exception::Exception& ex) {
        std::cout << ex.what() << std::endl;
    }
    glfwTerminate();
    return 0;
}