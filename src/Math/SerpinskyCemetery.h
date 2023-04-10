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

        explicit SerpinskyCemetery(int amountSteps) noexcept;
        ~SerpinskyCemetery() = default;
        SerpinskyCemetery(SerpinskyCemetery&&) = default;

        void render() noexcept;
        void set_WH(unsigned int width, unsigned int height) noexcept;
        unsigned char* get_pixels() noexcept;

    private:
        void gen_fractal(Square square, int currSteps) noexcept;
        void fill_pixels(const Square& square, Color color) noexcept;
        [[nodiscard]] unsigned int fix_amount_step(unsigned int amount_step) const noexcept;
        [[nodiscard]] unsigned int gen_size(unsigned int amount_step) const noexcept;

        unsigned int WIDTH;
        unsigned int HEIGHT;
        unsigned int _size;
        std::vector<std::vector<Color>> _pixels;
        std::shared_ptr<std::vector<unsigned char>> _data = nullptr;
    };
}
