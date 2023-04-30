#include "Image.h"

#include <stdexcept>
#include <iostream>

namespace Renderer {
    Image::Image(const unsigned int width, const unsigned int height) noexcept :
    WIDTH(width),
    HEIGHT(height),
    _data(WIDTH * HEIGHT * 3, 170) {}

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
}
