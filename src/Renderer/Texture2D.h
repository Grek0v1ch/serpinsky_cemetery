#pragma once

#include <glad/glad.h>
#include <glm/vec2.hpp>

#include <string>
#include <map>

namespace Renderer {
    class Texture2D {
    public:
        Texture2D() = delete;
        Texture2D(const Texture2D&) = delete;
        Texture2D& operator=(const Texture2D&) = delete;
        
    public:
        Texture2D(GLsizei width, GLsizei height,
                  const unsigned char* data, unsigned int channels = 3,
                  GLint filter = GL_NEAREST, GLint wrapMode = GL_CLAMP_TO_EDGE);
        ~Texture2D() noexcept;
        Texture2D& operator=(Texture2D&& o) noexcept;
        Texture2D(Texture2D&& o) noexcept;

    public:
        void bind() const noexcept;
        void unbind() const noexcept;
        GLsizei width() const noexcept { return _width; }
        GLsizei height() const noexcept { return _height; }

    private:
        GLuint _id;
        GLsizei _width;
        GLsizei _height;
    };
}