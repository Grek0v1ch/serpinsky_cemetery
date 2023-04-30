#pragma once

#include <vector>
#include <memory>

#include <glad/glad.h>

#include "Geometry.hpp"

namespace Renderer {
    class Image;
    class Sprite;
    enum class Color;

    struct ViewPort {
        Math::Vector leftBottom;
        Math::Vector rightTop;
    };
}

namespace Math {
    class SerpinskyCemetery {
    public:
        SerpinskyCemetery(const SerpinskyCemetery&) = delete;
        SerpinskyCemetery& operator=(const SerpinskyCemetery&) = delete;
        SerpinskyCemetery& operator=(SerpinskyCemetery&&) = delete;

    public:
        explicit SerpinskyCemetery(const Polygon& userPolygon, int amountSteps = 1,
                                   double ratio = 1.);
        SerpinskyCemetery(SerpinskyCemetery&&) = default;

    public:
        void render() noexcept;

        void setWH(unsigned int newWidth, unsigned int newHeight) noexcept;

        void setStep(unsigned int amountStep);

        void setRatio(double ratio);

        void setInitPolygon(const Polygon& initPolygon);

        void setViewPort(const Renderer::ViewPort& viewPort);

    private:
        void initSprite() noexcept;

        void makeFractal();

        void genFractal(const Polygon& square, int currSteps);

        void fillPolygon(const Polygon& o, Renderer::Color color);

        void fillTriangle(Vector v0, Vector v1, Vector v2, Renderer::Color color);

        void scaleTriangle(Vector& v0, Vector& v1, Vector& v2) const noexcept;

        double edge(Vector v0, Vector v1, Vector p) const noexcept;

        Vector divSegmentInRatio(const Vector& v0, const Vector& v1, double ratio) const noexcept;

        std::pair<Vector, Vector> getTwoPointsinRatio(const Vector& v0, const Vector& v1,
                                                    double ratio) const noexcept;

        Vector getIntersectionTwoSegment(const Vector& v0, const Vector& v1, const Vector& v2,
                                        const Vector& v3) const noexcept;
        [[nodiscard]]
        unsigned int fixAmountStep(unsigned int amountStep) noexcept;

    private:
        Renderer::ViewPort _viewPort;
        Polygon _initPolygon;
        unsigned int _amountSteps;
        double _ratio;
        std::shared_ptr<Renderer::Image> _img;
        std::shared_ptr<Renderer::Sprite> _sprite;
    };
}
