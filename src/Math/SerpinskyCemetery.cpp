#include "SerpinskyCemetery.h"

#include <memory>
#include <iostream>
#include <cmath>

#include "../ResourceManager/ResourceManager.h"
#include "../Renderer/Texture2D.h"
#include "../Renderer/Sprite.h"

namespace Math {
    SerpinskyCemetery::SerpinskyCemetery(int amountSteps) noexcept {
        _size = gen_size(fix_amount_step(amountSteps));
        WIDTH = HEIGHT = _size;
        _pixels = {_size, std::vector<Color>(_size, Color::BLACK)};
        gen_fractal({{0, 0}, {_size, _size}}, amountSteps);
    }

    void SerpinskyCemetery::gen_fractal(Square square, int currSteps) noexcept {
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
        if (currSteps > 0) {
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

    unsigned char *SerpinskyCemetery::get_pixels() noexcept {
        if (_data == nullptr) {
            _data = std::make_shared<std::vector<unsigned char>>();
            _data->reserve(_size * _size * 3);
            for (const auto& str : _pixels) {
                for (const auto& pixel : str) {
                    switch (pixel) {
                        case Color::BLACK:
                            _data->push_back(0);
                            _data->push_back(0);
                            _data->push_back(0);
                            break;
                        case Color::WHITE:
                            _data->push_back(255);
                            _data->push_back(255);
                            _data->push_back(255);
                            break;
                        default:
                            break;
                    }
                }
            }
        }
        return _data->data();
    }

    void SerpinskyCemetery::fill_pixels(const Square& square, Color color) noexcept {
        for (std::size_t x = square.leftBottom.x; x < square.rightTop.x; x++) {
            for (std::size_t y = square.leftBottom.y; y < square.rightTop.y; y++) {
                _pixels[x][y] = color;
            }
        }
    }

    unsigned int SerpinskyCemetery::fix_amount_step(unsigned int amount_step) const noexcept {
        if (amount_step > 8) {
            return 8;
        }
        else {
            return amount_step;
        }
    }

    unsigned int SerpinskyCemetery::gen_size(unsigned int amount_step) const noexcept {
        if (amount_step <= 5) {
            return 243;
        } else {
            return static_cast<unsigned int>(pow(3, amount_step));
        }
    }
}