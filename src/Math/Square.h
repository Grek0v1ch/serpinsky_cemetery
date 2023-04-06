#pragma once

#include <glm/vec2.hpp>
#include <glad/glad.h>

#include <memory>
#include <array>

#include "../Renderer/ShaderProgram.h"

namespace Math {
    class Square {
    private:
        glm::vec2 _right_top;
        glm::vec2 _left_bottom;
        std::shared_ptr<Renderer::ShaderProgram> _shader_program;
        GLuint _vao = 0;
        GLuint _vbo = 0;
        GLuint _ebo = 0;
        static constexpr uint8_t VERTICES_SIZE = 12;
        static constexpr uint8_t INDICES_SIZE = 6;

        void init_buffers(const std::array<GLfloat, VERTICES_SIZE>& vertices,
                          const std::array<GLuint, INDICES_SIZE>& indices) noexcept;

    public:
        Square(const glm::vec2& right_top,
               const glm::vec2& left_bottom,
               std::shared_ptr<Renderer::ShaderProgram> shader_program) noexcept;
        Square(const Square& o) noexcept;
        Square(Square&& o) noexcept;
        ~Square();

        Square& operator=(const Square&) noexcept;

        void render() const;
        [[nodiscard]]
        double side_length() const noexcept;
        glm::vec2 center() const noexcept;
    };
}
