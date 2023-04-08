#pragma once

#include <vector>
#include <memory>

#include <glad/glad.h>

namespace Renderer {
    class ShaderProgram;
}

namespace Math {
    struct Coord {
        unsigned int x;
        unsigned int y;
    };

    struct Square {
        Coord leftBottom;
        Coord rightTop;
    };

    enum class Color {
        WHITE,
        BLACK
    };

    class SerpinskyCemetery {
    public:
        SerpinskyCemetery(const SerpinskyCemetery&) = delete;
        SerpinskyCemetery& operator=(const SerpinskyCemetery&) = delete;
        SerpinskyCemetery& operator=(SerpinskyCemetery&&) = delete;

        SerpinskyCemetery(Coord leftBottom, Coord rightTop, int amountSteps,
                          const std::string& shaderName) noexcept;
        ~SerpinskyCemetery() = default;
        SerpinskyCemetery(SerpinskyCemetery&&) = default;

        void render() noexcept;
        unsigned char* get_pixels() noexcept;

    private:
        void gen_fractal(Square square, int currSteps) noexcept;
        void feel_pixels(const Square& square, Color color) noexcept;

        const unsigned int WIDTH;
        const unsigned int HEIGHT;
        std::vector<std::vector<Color>> _pixels;
        std::shared_ptr<std::vector<unsigned char>> _data = nullptr;
    };
}
