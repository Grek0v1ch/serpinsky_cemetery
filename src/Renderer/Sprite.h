#pragma once

#include <glad/glad.h>
#include <glm/vec2.hpp>

#include <memory>
#include <string>

namespace Renderer {
    class Texture2D;
    class ShaderProgram;
    class Sprite {
    public:
        Sprite(const Sprite&) = delete;
        Sprite& operator=(const Sprite&) = delete;

    public:
        Sprite(const std::shared_ptr<Texture2D> texture,
               const std::shared_ptr<ShaderProgram> shaderProgram,
               const glm::vec2& position = glm::vec2{0.0f},
               const glm::vec2& size = glm::vec2{1.0f},
               float rotation = 0.0);
        ~Sprite();

    public:
       void render() const;

       void setSize(const glm::vec2& size);

        glm::vec2 size() const noexcept { return  _size; }

    private:
        std::shared_ptr<Texture2D> _texture;
        std::shared_ptr<ShaderProgram> _shaderProgram;
        glm::vec2 _position;
        glm::vec2 _size;
        float _rotation;
        GLuint _VAO;
        GLuint _vertexCoordsVBO;
        GLuint _textureCoordsVBO;
    };
}