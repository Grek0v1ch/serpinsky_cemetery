#include "Image.h"

#include <stdexcept>
#include <iostream>

namespace Renderer {
    Image::Image(const unsigned int width, const unsigned int height) noexcept :
    WIDTH(width),
    HEIGHT(height),
    SIZE(width * height * 3),
    _data(SIZE, 0) {}

    void Image::setColor(unsigned int x, unsigned int y, Color color) {
        if (x >= WIDTH or y >= HEIGHT) {
            throw std::out_of_range("Out of range in image");
        }
        size_t idx = y * (WIDTH * 3) + x * 3;
        switch (color) {
            case Color::BLACK:
                _data[idx] = 0;
                _data[idx + 1] = 0;
                _data[idx + 2] = 0;
                break;
            case Color::WHITE:
                _data[idx] = 255;
                _data[idx + 1] = 255;
                _data[idx + 2] = 255;
                break;
            default:
                break;
        }
    }

    void Image::print() const noexcept {
        for (size_t j = 0; j < HEIGHT; j++) {
            for (size_t i = 0; i < WIDTH * 3; i += 3) {
                std::cout << (int)_data[j * WIDTH * 3 + i]
                          << (int)_data[j * WIDTH * 3 + i + 1]
                          << (int)_data[j * WIDTH * 3 + i + 2]
                          << ' ';
            }
            std::cout << '\n';
        }
    }
}
