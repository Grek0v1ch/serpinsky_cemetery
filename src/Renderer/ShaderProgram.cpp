#include "ShaderProgram.h"

#include <iostream>

#include <glm/gtc/type_ptr.hpp>

#include "../Exception/Exception.h"

namespace Renderer {
    ShaderProgram::ShaderProgram(const std::string& vertexShaderSource,
                                 const std::string& fragmentShaderSource) : _programID(0) {
        // Сначала нужно скомпилировать вершинный и фрагментный шейдеры
        GLuint vertexShader;
        try {
            vertexShader = createShader(vertexShaderSource, GL_VERTEX_SHADER);
        } catch (Exception::Exception& ex) {
            std::string msg = std::string{"Error in ShaderProgram: "} + ex.what();
            throw Exception::Exception(msg);
        }
        GLuint fragmentShader;
        try {
            fragmentShader = createShader(fragmentShaderSource, GL_FRAGMENT_SHADER);
        } catch (Exception::Exception& ex) {
            glDeleteShader(vertexShader);
            std::string msg = std::string{"Error in ShaderProgram: "} + ex.what();
            throw Exception::Exception(msg);
        }
        // Создаем объект программы и собираем шейдеры
        _programID = glCreateProgram();
        glAttachShader(_programID, vertexShader);
        glAttachShader(_programID, fragmentShader);
        glLinkProgram(_programID);
        // Чистим память
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        // Сообщаем об ошибках, если они есть
        GLint success;
        GLchar infoLog[1024];
        glGetProgramiv(_programID, GL_LINK_STATUS, &success);
        if (! success) {
            glGetProgramInfoLog(_programID, 1024, nullptr, infoLog);
            std::string msg = std::string{"Linking shader program error: "} + infoLog;
            throw Exception::Exception(msg);
        }
    }

    ShaderProgram::ShaderProgram(Renderer::ShaderProgram&& shaderProgram)
    noexcept : _programID(shaderProgram._programID) {
        shaderProgram._programID = 0;
    }

    ShaderProgram::~ShaderProgram() {
        glDeleteProgram(_programID);
    }

    void ShaderProgram::use() const {
        glUseProgram(_programID);
    }

    void ShaderProgram::setUniform(const std::string& name, const glm::mat4& matrix) {
        auto location = glGetUniformLocation(_programID, name.c_str());
        if (location == -1) {
            std::cerr << "Not find uniform " << name << std::endl;
            return;
        }
        use();
        glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
    }

     GLuint ShaderProgram::createShader(const std::string& source, GLenum shaderType) {
        // Создаем объект шейдера
        GLuint shader = glCreateShader(shaderType);
        const GLchar *shader_source = source.c_str();
        glShaderSource(shader, 1, &shader_source, nullptr);
        // Компилируем шейдер
        glCompileShader(shader);
        // Далее получаем сообщение об ошибке (если оно есть)
        GLint success;
        GLchar infoLog[1024];
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (! success) {
            glGetShaderInfoLog(shader, 1024, nullptr, infoLog);
            // В зависимости от типа шейдера сообщаем об ошибке
            std::string msg;
            switch (shaderType) {
                case GL_VERTEX_SHADER:
                    msg = std::string{"Compile vertex shader error\n"} + std::string{infoLog};
                    throw Exception::Exception(msg);
                case GL_FRAGMENT_SHADER:
                    msg = std::string{"Compile fragment shader error\n"} + std::string{infoLog};
                    throw Exception::Exception(msg);
                default:
                    msg = std::string{"Compile shader error\n"} + std::string{infoLog};
                    throw Exception::Exception(msg);
            }
        }
        return shader;
    }
}