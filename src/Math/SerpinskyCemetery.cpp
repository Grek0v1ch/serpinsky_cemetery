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

    SerpinskyCemetery::SerpinskyCemetery(int amountSteps) noexcept :
    _size(gen_size(fix_amount_step(amountSteps))) {
        WIDTH = HEIGHT = _size;
        _pixels = std::make_shared<Image>(_size, _size);
        gen_fractal({{0, 0}, {_size, _size}}, amountSteps);
    }

    void SerpinskyCemetery::gen_fractal(Square square, int currSteps) noexcept {
        if (currSteps <= 0) {
            return;
        }
        unsigned int dx = square.rightTop.x - square.leftBottom.x;
        unsigned int dy = square.rightTop.y - square.leftBottom.y;
        Square square1 {{square.leftBottom.x, square.leftBottom.y + dy / 3},
                        {square.leftBottom.x + dx / 3, square.leftBottom.y + dy / 3 * 2}};
        Square square2 {{square.leftBottom.x + dx / 3, square.leftBottom.y},
                        {square.leftBottom.x + dx / 3 * 2, square.rightTop.y}};
        Square square3 {{square.leftBottom.x + dx / 3 * 2, square.leftBottom.y + dy / 3},
                        {square.rightTop.x, square.leftBottom.y + dy / 3 * 2}};
        fill_pixels(square1, Color::WHITE);
        fill_pixels(square2, Color::WHITE);
        fill_pixels(square3, Color::WHITE);
        currSteps--;
        gen_fractal({{square.leftBottom.x, square.leftBottom.y + dy / 3 * 2},
                     {square.leftBottom.x + dx / 3, square.rightTop.y}},
                    currSteps);
        gen_fractal({{square.leftBottom},
                     {square.leftBottom.x + dx / 3, square.leftBottom.y + dy / 3}},
                    currSteps);
        gen_fractal({{square.leftBottom.x + dx / 3 * 2, square.leftBottom.y},
                     {square.rightTop.x, square.leftBottom.y + dy / 3}},
                    currSteps);
        gen_fractal({{square.leftBottom.x + dx / 3 * 2, square.leftBottom.y + dy / 3 * 2},
                     {square.rightTop}}, currSteps);
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

    void SerpinskyCemetery::fill_pixels(const Square& square, Color color) noexcept {
        fill_polygon({{(double)square.leftBottom.x, (double)square.leftBottom.y}, 
                      {(double)square.leftBottom.x, (double)square.rightTop.y},
                      {(double)square.rightTop.x, (double)square.rightTop.y},
                      {(double)square.rightTop.x, (double)square.leftBottom.y}},
                      color);
    }

    void SerpinskyCemetery::fill_polygon(const Polygon& o, Renderer::Color color) noexcept {
        fill_triangle(o.a, o.b, o.c, color);
        fill_triangle(o.c, o.d, o.a, color);
    }

    void SerpinskyCemetery::fill_triangle(
        Vertex v0, 
        Vertex v1, 
        Vertex v2, 
        Renderer::Color color
    ) noexcept {
        int min_x = ceil(std::min(std::min(v0.position.x, v1.position.x), v2.position.x));
        int max_x = ceil(std::max(std::max(v0.position.x, v1.position.x), v2.position.x));
        int min_y = ceil(std::min(std::min(v0.position.y, v1.position.y), v2.position.y));
        int max_y = ceil(std::max(std::max(v0.position.y, v1.position.y), v2.position.y));
        for (int x = min_x; x < max_x; ++x) {
            for (int y = min_y; y < max_y; ++y) {
                Point p = {static_cast<double>(x), static_cast<double>(y)};
                double e10 = edge(v1.position, v0.position, p);
                double e21 = edge(v2.position, v1.position, p);
                double e02 = edge(v0.position, v2.position, p);
                if (e10 >= 0 and e21 >= 0 and e02 >= 0) {
                    _pixels->setColor(x, y, color);
                }
            }
        }
    }

    double SerpinskyCemetery::edge(Point v0, Point v1, Point p) {
            Vector a = p - v0;
            Vector b = v1 - v0;
            return a.y * b.x - a.x * b.y;
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