#pragma once

#include <vector>
#include <memory>

#include <glm/vec2.hpp>

namespace Renderer {
    class ShaderProgram;
}

namespace Math {
    class Square;

    class TSquareFractal {
    private:
        std::vector<Square> _squares;
        std::shared_ptr<Renderer::ShaderProgram> _shader_program;

        void gen_fractal(const glm::vec2& centre, double length, int iteration);
    public:
        TSquareFractal() = delete;
        TSquareFractal(const glm::vec2& centre,
                       double length,
                       int iteration,
                       std::shared_ptr<Renderer::ShaderProgram> shader_program) noexcept;

        TSquareFractal(const TSquareFractal&) = delete;
        TSquareFractal(TSquareFractal&&) = delete;
        TSquareFractal& operator=(const TSquareFractal&) = delete;
        TSquareFractal& operator=(TSquareFractal&&) = delete;

        void render() const noexcept;
    };
}
