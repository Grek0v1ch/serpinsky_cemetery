#pragma once

#include <cctype>
#include <vector>

namespace Renderer {
    enum class Color {
        WHITE,
        BLACK
    };

    class Image {
    public:
        Image(const Image&) = delete;
        Image(Image&&) = delete;
        Image& operator=(const Image&) = delete;
        Image& operator=(Image&&) = delete;

        Image(unsigned int width, unsigned int height) noexcept;

        void setColor(unsigned int x, unsigned int y, Color color);
        [[nodiscard]]
        const unsigned char* getData() const noexcept { return _data.data(); }

        void print() const noexcept;

    private:
        const unsigned int WIDTH;
        const unsigned int HEIGHT;
        const size_t SIZE;
        std::vector<unsigned char> _data;
    };
}