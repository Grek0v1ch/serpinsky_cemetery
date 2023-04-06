#pragma once

#include <vector>

#include <glad/glad.h>

namespace Renderer {
    class ShaderProgram;
}

namespace Math {
    struct Coord {
        GLfloat x;
        GLfloat y;
    };

    struct Square {
        Coord leftBottom;
        Coord rightTop;
    };

    class SerpinskyCemetery {
    public:
        SerpinskyCemetery(Coord leftBottom, Coord rightTop, int amountSteps,
                          const std::string& shaderName) noexcept;
        ~SerpinskyCemetery();

        void render() const noexcept;
    private:
        void init_buffers() noexcept;
        void gen_fractal(Square square, int currSteps) noexcept;
        [[nodiscard]] std::shared_ptr<std::vector<GLfloat>> makeVectorCoords() const noexcept;

        std::vector<Square> _squares;
        std::shared_ptr<Renderer::ShaderProgram> _shader_program;
        GLuint _vao = 0;
        GLuint _vbo = 0;
    };
}
