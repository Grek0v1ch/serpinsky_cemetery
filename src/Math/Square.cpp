#include "Square.h"

#include <utility>

namespace Math {
    void Square::init_buffers(const std::array<GLfloat, VERTICES_SIZE>& vertices,
                              const std::array<GLuint, INDICES_SIZE>& indices) noexcept {
        glGenVertexArrays(1, &_vao);
        glGenBuffers(1, &_vbo);
        glGenBuffers(1, &_ebo);
        glBindVertexArray(_vao);
        glBindBuffer(GL_ARRAY_BUFFER, _vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices.data(), GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), nullptr);
        glEnableVertexAttribArray(0);
        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    Square::Square(const glm::vec2& right_top,
                   const glm::vec2& left_bottom,
                   std::shared_ptr<Renderer::ShaderProgram> shader_program) noexcept :
    _right_top(right_top),
    _left_bottom(left_bottom),
    _shader_program(std::move(shader_program)) {
        std::array<GLuint, INDICES_SIZE> indices {
            0, 1, 2,
            2, 3, 0
        };
        std::array<GLfloat, VERTICES_SIZE> vertices {
            _left_bottom.x, _left_bottom.y, 0.0,
            _left_bottom.x, _right_top.y,   0.0,
            _right_top.x,   _right_top.y,   0.0,
            _right_top.x,   _left_bottom.y, 0.0
        };
        init_buffers(vertices, indices);
    }

    Square::Square(const Square& o) noexcept : Square(o._right_top,
                                                      o._left_bottom,
                                                      o._shader_program) {
        std::array<GLuint, INDICES_SIZE> indices {
            0, 1, 2,
            2, 3, 0
        };
        std::array<GLfloat, VERTICES_SIZE> vertices {
            _left_bottom.x, _left_bottom.y, 0.0,
            _left_bottom.x, _right_top.y,   0.0,
            _right_top.x,   _right_top.y,   0.0,
            _right_top.x,   _left_bottom.y, 0.0
        };
        init_buffers(vertices, indices);
    }

    Square::Square(Math::Square&& o) noexcept :
    _right_top(o._right_top),
    _left_bottom(o._left_bottom),
    _shader_program(o._shader_program),
    _vao(o._vao),
    _vbo(o._vbo),
    _ebo(o._ebo) {
        _right_top = glm::vec2(0);
        _left_bottom = glm::vec2(0);
        o._shader_program = nullptr;
        o._vao = 0;
        o._vbo = 0;
        o._ebo = 0;
    }

    Square::~Square() {
        glDeleteBuffers(1, &_ebo);
        glDeleteBuffers(1, &_vbo);
        glDeleteBuffers(1, &_vao);
    }

    Square& Square::operator=(const Math::Square& o) noexcept {
        if (this == &o) {
            return *this;
        }
        _left_bottom = o._left_bottom;
        _right_top = o._right_top;
        _shader_program = o._shader_program;
        std::array<GLuint, INDICES_SIZE> indices {
            0, 1, 2,
            2, 3, 0
        };
        std::array<GLfloat, VERTICES_SIZE> vertices {
            _left_bottom.x, _left_bottom.y, 0.0,
            _left_bottom.x, _right_top.y,   0.0,
            _right_top.x,   _right_top.y,   0.0,
            _right_top.x,   _left_bottom.y, 0.0
        };
        init_buffers(vertices, indices);
        return *this;
    }

    void Square::render() const {
            _shader_program->use();
            glBindVertexArray(_vao);
            glDrawElements(GL_TRIANGLES, INDICES_SIZE, GL_UNSIGNED_INT, nullptr);
            glBindVertexArray(0);
    }

    double Square::side_length() const noexcept {
        return _right_top.x - _left_bottom.x;
    }

    glm::vec2 Square::center() const noexcept {
        return {(_left_bottom.x + _right_top.x) / 2, (_left_bottom.y + _right_top.y) / 2};
    }
}
