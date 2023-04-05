#include <iostream>
#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/vec2.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Exception/Exception.h"
#include "Renderer/ShaderProgram.h"
#include "Math/Square.h"
#include "Math/TSquareFractal.h"

const std::string vertex_source {
    R"(
#version 330 core
layout (location = 0) in vec3 position;
uniform mat4 projectionMatrix;
uniform mat4 modelMatrix;
void main()
{
    gl_Position = projectionMatrix * modelMatrix * vec4(position.x, position.y, position.z, 1.0);
}
)"
};

const std::string fragment_source {
    R"(
#version 330 core
out vec4 color;
void main()
{
	color = vec4(255.0 / 255, 255.0 / 255, 255.0 / 255, 1.0f);
}
)"
};

const GLuint WIDTH = 800, HEIGHT = 800;

void glfwKeyCallback(GLFWwindow* pWindow, int key, int scancode, int action, int mode) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(pWindow, GL_TRUE);
    }
}

int main() {
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
    modelMatrix = glm::translate(modelMatrix, glm::vec3(WIDTH / 2, HEIGHT / 2, 0.0f));
    try {
        auto shader_program = std::make_shared<Renderer::ShaderProgram>(vertex_source,
                                                                        fragment_source);
        shader_program->setUniform("projectionMatrix", projectionMatrix);
        shader_program->setUniform("modelMatrix", modelMatrix);

        Math::TSquareFractal fractal({0.0f, 0.0f},
                                     static_cast<double>(WIDTH) / 2,
                                     7,
                                     shader_program);

        glClear(GL_COLOR_BUFFER_BIT);
        fractal.render();
        glfwSwapBuffers(window);

        while (!glfwWindowShouldClose(window)) {
            glfwPollEvents();
        }
    } catch (Exception::Exception& ex) {
        std::cout << ex.what() << std::endl;
    }
    glfwTerminate();
    return 0;
}