#pragma once

#include <glad/glad.h>
#include <glm/mat4x4.hpp>

#include <string>

namespace Renderer {
    class ShaderProgram {
    public:
        ShaderProgram() = delete;
        ShaderProgram(const ShaderProgram&) = delete;
        ShaderProgram& operator=(const ShaderProgram&) = delete;
    public:
        ShaderProgram(const std::string& vertexShaderSource,
                      const std::string& fragmentShaderSource);

        ShaderProgram(Renderer::ShaderProgram&& shaderProgram) noexcept;

        ~ShaderProgram();

    public:
        void use() const;

        void setUniform(const std::string& name, const glm::mat4& matrix);

    private:
        static GLuint createShader(const std::string& source, GLenum shaderType);

        GLuint _programID;
    };
}