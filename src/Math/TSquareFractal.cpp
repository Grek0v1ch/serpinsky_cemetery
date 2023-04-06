#include "TSquareFractal.h"

#include <utility>

#include "Square.h"

namespace Math {
    void TSquareFractal::gen_fractal(const glm::vec2& centre, const double length, const int iteration) {
        double x1 = centre.x - length / 2;
        double x2 = centre.x + length / 2;
        double y1 = centre.y - length / 2;
        double y2 = centre.y + length / 2;
        _squares.emplace_back(glm::vec2(x1, y1), glm::vec2(x2, y2), _shader_program);
        if (iteration  && (length / 2) > 1.0) {
            gen_fractal({x1, y1}, length / 2, iteration - 1);
            gen_fractal({x1, y2}, length / 2, iteration - 1);
            gen_fractal({x2, y1}, length / 2, iteration - 1);
            gen_fractal({x2, y2}, length / 2, iteration - 1);
        }
    }

    TSquareFractal::TSquareFractal(const glm::vec2& centre,
                                   const double length,
                                   const int iterations,
                                   const std::shared_ptr<Renderer::ShaderProgram>& shader_program)
                                   noexcept :
    _squares{},
    _shader_program(shader_program) {
        gen_fractal(centre, length, iterations);
    }

    void TSquareFractal::render() const noexcept {
        for (const auto& square : _squares) {
            square.render();
        }
    }
}
