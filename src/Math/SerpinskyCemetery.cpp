#include "SerpinskyCemetery.h"

#include <memory>
#include <iostream>
#include <cmath>

#include "../ResourceManager/ResourceManager.h"
#include "../Renderer/Texture2D.h"
#include "../Renderer/Sprite.h"
#include "../Renderer/Image.h"

namespace Math {
    using namespace Renderer;

    SerpinskyCemetery::SerpinskyCemetery(const Polygon initPolygon, int amountSteps = 1)
    noexcept : _size(gen_size(fix_amount_step(amountSteps)))
             , WIDTH(_size)
             , HEIGHT(_size)
             , _pixels(std::make_shared<Image>(_size, _size)) {
        double width = static_cast<double>(WIDTH);
        double height = static_cast<double>(HEIGHT);
        Polygon scale_init_polygon = {{initPolygon.a.x * width, initPolygon.a.y * height},
                                      {initPolygon.b.x * width, initPolygon.b.y * height},
                                      {initPolygon.c.x * width, initPolygon.c.y * height},
                                      {initPolygon.d.x * width, initPolygon.d.y * height}};
        fill_polygon(scale_init_polygon, Color::BLACK);
        gen_fractal(scale_init_polygon,amountSteps);
    }

    void SerpinskyCemetery::gen_fractal(const Polygon polygon, int currSteps) noexcept {
        double ratio = 1.;
        if (currSteps <= 0) {
            return;
        }
        auto ab_points = get_2_points_in_ratio(polygon.a, polygon.b, ratio);
        auto bc_points = get_2_points_in_ratio(polygon.b, polygon.c, ratio);
        auto cd_points = get_2_points_in_ratio(polygon.c, polygon.d, ratio);
        auto da_points = get_2_points_in_ratio(polygon.d, polygon.a, ratio);

        auto v0 = get_intersection_2_segment(bc_points.first, da_points.second,
                                             ab_points.second, cd_points.first);
        auto v1 = get_intersection_2_segment(bc_points.second, da_points.first,
                                             ab_points.second, cd_points.first);
        auto v3 = get_intersection_2_segment(bc_points.first, da_points.second,
                                             ab_points.first, cd_points.second);
        auto v2 = get_intersection_2_segment(bc_points.second, da_points.first,
                                             ab_points.first, cd_points.second);

        fill_polygon({ab_points.first, ab_points.second, v0, v3}, Color::WHITE);
        fill_polygon({bc_points.first, bc_points.second, da_points.first, da_points.second},
                     Color::WHITE);
        fill_polygon({v2, v1, cd_points.first, cd_points.second}, Color::WHITE);

        gen_fractal({ab_points.second, polygon.b, bc_points.first, v0}, currSteps - 1);
        gen_fractal({bc_points.second, polygon.c, cd_points.first, v1}, currSteps - 1);
        gen_fractal({da_points.first, v2, cd_points.second, polygon.d}, currSteps - 1);
        gen_fractal({polygon.a, ab_points.first, v3, da_points.second}, currSteps - 1);
    }

    void SerpinskyCemetery::render() noexcept {
        using Renderer::Texture2D;
        using Renderer::Sprite;
        std::shared_ptr<Texture2D> texture = std::make_shared<Texture2D>(
                _size, _size, get_pixels());
        ResourceManager& res = ResourceManager::instance();
        std::shared_ptr<Sprite> sprite = std::make_shared<Sprite>(
                texture, res.getShaderProgram("SpriteShader"),
                glm::vec2{0.0f}, glm::vec2{WIDTH, HEIGHT});
        sprite->render();
    }

    void SerpinskyCemetery::set_WH(unsigned int width, unsigned int height) noexcept {
        WIDTH = width;
        HEIGHT = height;
    }

    const unsigned char *SerpinskyCemetery::get_pixels() noexcept {
        return _pixels->getData();
    }

    void SerpinskyCemetery::fill_polygon(const Polygon& o, Color color) noexcept {
        fill_triangle(o.a, o.b, o.c, color);
        fill_triangle(o.c, o.d, o.a, color);
    }

    void SerpinskyCemetery::fill_triangle(Point v0, Point v1, Point v2, Color color) noexcept {
        int min_x = ceil(std::min(std::min(v0.x, v1.x), v2.x));
        int max_x = ceil(std::max(std::max(v0.x, v1.x), v2.x));
        int min_y = ceil(std::min(std::min(v0.y, v1.y), v2.y));
        int max_y = ceil(std::max(std::max(v0.y, v1.y), v2.y));
        for (int x = min_x; x < max_x; ++x) {
            for (int y = min_y; y < max_y; ++y) {
                Point p = {static_cast<double>(x), static_cast<double>(y)};
                double e10 = edge(v1, v0, p);
                double e21 = edge(v2, v1, p);
                double e02 = edge(v0, v2, p);
                if (e10 >= 0 and e21 >= 0 and e02 >= 0) {
                    _pixels->setColor(x, y, color);
                }
            }
        }
    }

    double SerpinskyCemetery::edge(Point v0, Point v1, Point p) const noexcept {
            Vector a = p - v0;
            Vector b = v1 - v0;
            return a.y * b.x - a.x * b.y;
    }

    Point SerpinskyCemetery::div_segment_in_ratio(const Point v0,
                                                  const Point v1,
                                                  const double ratio) const noexcept {
        double x = (v0.x + ratio * v1.x) / (1 + ratio);
        double y = (v0.y + ratio * v1.y) / (1 + ratio);
        return {x, y};
    }

    std::pair<Point, Point>
    SerpinskyCemetery::get_2_points_in_ratio(const Point v0,
                                             const Point v1,
                                             const double ratio) const noexcept {
        Point p0 = div_segment_in_ratio(v0, v1, ratio / (1 + ratio));
        Point p1 = div_segment_in_ratio(v0, v1, (ratio + 1) / ratio);
        return {p0, p1};
    }

    Point SerpinskyCemetery::get_intersection_2_segment(const Point v0,
                                                        const Point v1,
                                                        const Point v2,
                                                        const Point v3) const noexcept {
        double denom = (v0.x - v1.x) * (v2.y - v3.y) - (v0.y - v1.y) * (v2.x - v3.x);
        double x = (v0.x * v1.y - v0.y * v1.x) * (v2.x - v3.x)
                 - (v0.x - v1.x) * (v2.x * v3.y - v2.y * v3.x);
        double y = (v0.x * v1.y - v0.y * v1.x) * (v2.y - v3.y)
                 - (v0.y - v1.y) * (v2.x * v3.y - v2.y * v3.x);
        x /= denom;
        y /= denom;
        return {x, y};
    }

    unsigned int SerpinskyCemetery::fix_amount_step(unsigned int amount_step) noexcept {
        if (amount_step > 8) {
            return 8;
        }
        else {
            return amount_step;
        }
    }

    unsigned int SerpinskyCemetery::gen_size(unsigned int amount_step) noexcept {
        if (amount_step <= 5) {
            return 243;
        } else {
            return static_cast<unsigned int>(pow(3, amount_step));
        }
    }
}