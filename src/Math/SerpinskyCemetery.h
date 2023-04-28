#pragma once

#include <vector>
#include <memory>

#include <glad/glad.h>

namespace Renderer {
    class Image;
    enum class Color;
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

    struct Vector {
        double x;
        double y;
    };

    struct Point {
        double x;
        double y;

        Vector operator-(const Point& rhs) {
            return {x - rhs.x, y - rhs.y};
        }
    };

    struct Vertex {
        Point position;
    };

    struct Polygon {
        Vertex a;
        Vertex b;
        Vertex c;
        Vertex d;
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
        const unsigned char* get_pixels() noexcept;

    private:
        void gen_fractal(Square square, int currSteps) noexcept;
        void fill_pixels(const Square& square, Renderer::Color color) noexcept;
        void fill_polygon(const Polygon& o, Renderer::Color color) noexcept;
        void fill_triangle(Vertex v0, Vertex v1, Vertex v2, Renderer::Color color) noexcept;
        double edge(Point v0, Point v1, Point p);
        [[nodiscard]]
        static unsigned int fix_amount_step(unsigned int amount_step) noexcept;
        [[nodiscard]]
        static unsigned int gen_size(unsigned int amount_step) noexcept;

        const unsigned int _size;
        unsigned int WIDTH;
        unsigned int HEIGHT;
        std::shared_ptr<Renderer::Image> _pixels;
    };
}
