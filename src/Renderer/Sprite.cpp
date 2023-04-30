#include "Sprite.h"

#include "ShaderProgram.h"
#include "Texture2D.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Renderer {
    Sprite::Sprite(const std::shared_ptr<Texture2D> pTexture,
                   const std::shared_ptr<ShaderProgram> pShaderProgram,
                   const glm::vec2& position,
                   const glm::vec2& size,
                   const float rotation)
    : _texture(std::move(pTexture))
    , _shaderProgram(std::move(pShaderProgram))
    , _position(position)
    , _size(size)
    , _rotation(rotation) {
        const GLfloat vertexCoords[] {
            // 2--3  4
            // | / / |
            // 1  6--5

            // x    y
            0.0f, 0.0f, // 1
            0.0f, 1.0f, // 2
            1.0f, 1.0f, // 3

            1.0f, 1.0f, // 4
            1.0f, 0.0f, // 5
            0.0f, 0.0f  // 6
        };

        glm::vec2 leftBottomUV {0.0f};
        glm::vec2 rightTopUV {1.0f};

        const GLfloat textureCoords[] {
            // u    v
            leftBottomUV.x, leftBottomUV.y,
            leftBottomUV.x, rightTopUV.y,
            rightTopUV.x, rightTopUV.y,

            rightTopUV.x, rightTopUV.y,
            rightTopUV.x, leftBottomUV.y,
            leftBottomUV.x, leftBottomUV.y,
        };

        glGenVertexArrays(1, &_VAO);
        glBindVertexArray(_VAO);

        glGenBuffers(1, &_vertexCoordsVBO);
        glBindBuffer(GL_ARRAY_BUFFER, _vertexCoordsVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertexCoords), &vertexCoords, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

        glGenBuffers(1, &_textureCoordsVBO);
        glBindBuffer(GL_ARRAY_BUFFER, _textureCoordsVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(textureCoords), &textureCoords, GL_STATIC_DRAW);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    Sprite::~Sprite() {
        glDeleteBuffers(1, &_vertexCoordsVBO);
        glDeleteBuffers(1, &_textureCoordsVBO);
        glDeleteVertexArrays(1, &_VAO);
    }

    void Sprite::render() const {
        _shaderProgram->use();

        glm::mat4 model(1.0f);

        // перемещаем на нужную позицию
        model = glm::translate(model, glm::vec3(_position, 0.0f));
        // перемещаем ориджин в левый нижний угол
        model = glm::translate(model, glm::vec3(0.5f * _size.x, 0.5f * _size.y, 0.0f));
        // поворачиваем
        model = glm::rotate(model, glm::radians(_rotation), glm::vec3(0.0f, 0.0f, 1.0f));
        // перемещаем ориджин в центр
        model = glm::translate(model, glm::vec3(-0.5f * _size.x, -0.5f * _size.y, 0.0f));
        // скалирование (увеличение размера)
        model = glm::scale(model, glm::vec3(_size, 1.0f));

        glBindVertexArray(_VAO);
        _shaderProgram->setUniform("modelMatrix", model);

        glActiveTexture(GL_TEXTURE0);
        _texture->bind();
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);
    }

    void Sprite::setSize(const glm::vec2& size) {
        _size = size;
    }
}