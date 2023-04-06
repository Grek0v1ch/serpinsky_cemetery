#include "SerpinskyCemetery.h"

#include "../ResourceManager/ResourceManager.h"
#include "../Renderer/ShaderProgram.h"

namespace Math {
    SerpinskyCemetery::SerpinskyCemetery(const Coord leftBottom,
                                         const Coord rightTop,
                                         int amountSteps,
                                         const std::string& shaderName) noexcept {
        gen_fractal({leftBottom, rightTop}, amountSteps);
        init_buffers();
        ResourceManager& resourceManager = ResourceManager::instance();
        _shader_program = resourceManager.getShaderProgram(shaderName);
    }

    SerpinskyCemetery::~SerpinskyCemetery() {
        glDeleteBuffers(1, &_vbo);
        glDeleteBuffers(1, &_vao);
    }

    void SerpinskyCemetery::render() const noexcept {
            _shader_program->use();
            glBindVertexArray(_vao);
            glDrawArrays(GL_TRIANGLES, 0, makeVectorCoords()->size());
            glBindVertexArray(0);
    }

    void SerpinskyCemetery::init_buffers() noexcept {
        auto vertices = makeVectorCoords();
        glGenVertexArrays(1, &_vao);
        glGenBuffers(1, &_vbo);
        glBindVertexArray(_vao);
        glBindBuffer(GL_ARRAY_BUFFER, _vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * vertices->size(), vertices->data(), GL_STATIC_DRAW);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    void SerpinskyCemetery::gen_fractal(Math::Square square, int currSteps) noexcept {
        GLfloat dx = square.rightTop.x - square.leftBottom.x;
        GLfloat dy = square.rightTop.y - square.leftBottom.y;
        if (dx < 1 or dy < 1) {
            return;
        }
        Square square1 {{square.leftBottom.x, square.leftBottom.y + 1.f / 3 * dy},
                        {square.leftBottom.x + 1.f / 3 * dx, square.leftBottom.y + 2.f / 3 * dy}};
        Square square2 {{square.leftBottom.x + 1.f / 3 * dx, square.leftBottom.y},
                        {square.leftBottom.x + 2.f / 3 * dx, square.rightTop.y}};
        Square square3 {{square.leftBottom.x + 2.f / 3 * dx, square.leftBottom.y + 1.f / 3 * dy},
                        {square.rightTop.x, square.leftBottom.y + 2.f / 3 * dy}};
        _squares.emplace_back(square1);
        _squares.emplace_back(square2);
        _squares.emplace_back(square3);
        currSteps--;
        if (currSteps > 0) {
            gen_fractal({{square.leftBottom.x, square.leftBottom.y + 2.f / 3 * dy},
                         {square.leftBottom.x + 1.f / 3 * dx, square.rightTop.y}},
                        currSteps);
            gen_fractal({{square.leftBottom},
                         {square.leftBottom.x + 1.f / 3 * dx, square.leftBottom.y + 1.f / 3 * dy}},
                        currSteps);
            gen_fractal({{square.leftBottom.x + 2.f / 3 * dx, square.leftBottom.y},
                         {square.rightTop.x, square.leftBottom.y + 1.f / 3 * dy}},
                        currSteps);
            gen_fractal({{square.leftBottom.x + 2.f / 3 * dx, square.leftBottom.y + 2.f / 3 * dy},
                         {square.rightTop}}, currSteps);
        }
    }

    std::shared_ptr<std::vector<GLfloat>> SerpinskyCemetery::makeVectorCoords() const noexcept {
        static std::shared_ptr<std::vector<GLfloat>> result = nullptr;
        if (result == nullptr) {
            result = std::make_shared<std::vector<GLfloat>>();
            for (const auto square: _squares) {
                result->push_back(square.leftBottom.x);
                result->push_back(square.leftBottom.y);
                result->push_back(square.leftBottom.x);
                result->push_back(square.rightTop.y);
                result->push_back(square.rightTop.x);
                result->push_back(square.rightTop.y);

                result->push_back(square.rightTop.x);
                result->push_back(square.rightTop.y);
                result->push_back(square.rightTop.x);
                result->push_back(square.leftBottom.y);
                result->push_back(square.leftBottom.x);
                result->push_back(square.leftBottom.y);
            }
        }
        return result;
    }
}
