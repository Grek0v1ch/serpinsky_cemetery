#pragma once

#include <vector>
#include <memory>

#include <glad/glad.h>

#include "Geometry.hpp"

namespace Renderer {
    class Image;
    enum class Color;
}

namespace Math {
    class SerpinskyCemetery {
    public:
        SerpinskyCemetery(const SerpinskyCemetery&) = delete;
        SerpinskyCemetery& operator=(const SerpinskyCemetery&) = delete;
        SerpinskyCemetery& operator=(SerpinskyCemetery&&) = delete;

    public:
        SerpinskyCemetery(const Polygon initPolygon, int amountSteps) noexcept;
        ~SerpinskyCemetery() = default;
        SerpinskyCemetery(SerpinskyCemetery&&) = default;

        void render() noexcept;
        void set_WH(unsigned int width, unsigned int height) noexcept;
        const unsigned char* get_pixels() noexcept;

    private:
        void gen_fractal(const Polygon square,
                         int currSteps) noexcept;

        void fill_polygon(const Polygon& o,
                          Renderer::Color color) noexcept;

        void fill_triangle(Point v0,
                           Point v1,
                           Point v2,
                           Renderer::Color color) noexcept;

        double edge(Point v0,
                    Point v1,
                    Point p) const noexcept;

        Point div_segment_in_ratio(const Point v0,
                                   const Point v1,
                                   const double ratio) const noexcept;

        std::pair<Point, Point> get_2_points_in_ratio(const Point v0,
                                                      const Point v1,
                                                      const double ratio) const noexcept;

        Point get_intersection_2_segment(const Point v0,
                                         const Point v1,
                                         const Point v2,
                                         const Point v3) const noexcept;
        [[nodiscard]]
        static unsigned int fix_amount_step(unsigned int amount_step) noexcept;

        [[nodiscard]] static unsigned int gen_size(unsigned int amount_step) noexcept;

    private:
        const unsigned int _size;
        unsigned int WIDTH;
        unsigned int HEIGHT;
        std::shared_ptr<Renderer::Image> _pixels;
    };
}
