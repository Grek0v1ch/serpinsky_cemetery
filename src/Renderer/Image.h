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
    public:
        Image(unsigned int width, unsigned int height) noexcept;

    public:
        unsigned int width() const noexcept { return WIDTH; }

        unsigned int height() const noexcept { return HEIGHT; }

        void setColor(unsigned int x, unsigned int y, Color color);

        void clear() noexcept;

        [[nodiscard]]
        const unsigned char* data() const noexcept { return _data.data(); }

    private:
        const unsigned int WIDTH;
        const unsigned int HEIGHT;
        std::vector<unsigned char> _data;
    };
}