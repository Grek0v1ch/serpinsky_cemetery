#pragma once

#include <vector>
#include <memory>

#include <glad/glad.h>

#include "Geometry.hpp"

namespace Renderer {
    class Image;
    class Sprite;
    enum class Color;
}

namespace Math {
    class SerpinskyCemetery {
    public:
        SerpinskyCemetery(const SerpinskyCemetery&) = delete;
        SerpinskyCemetery& operator=(const SerpinskyCemetery&) = delete;
        SerpinskyCemetery& operator=(SerpinskyCemetery&&) = delete;

    public:
        SerpinskyCemetery(const Polygon& userPolygon, int amountSteps = 1, double ratio = 1.);
        SerpinskyCemetery(SerpinskyCemetery&&) = default;

    public:
        void render() noexcept;

        void setWH(unsigned int newWidth, unsigned int newHeight) noexcept;

        void setStep(unsigned int amountStep);

        void setRatio(double ratio);

    private:
        void initSprite() noexcept;

        void genFractal(const Polygon& square, int currSteps);

        void fillPolygon(const Polygon& o, Renderer::Color color);

        void fillTriangle(Point v0, Point v1, Point v2, Renderer::Color color);

        double edge(Point v0, Point v1, Point p) const noexcept;

        Point divSegmentInRatio(const Point& v0, const Point& v1, double ratio) const noexcept;

        std::pair<Point, Point> getTwoPointsinRatio(const Point& v0, const Point& v1,
                                                    double ratio) const noexcept;

        Point getIntersectionTwoSegment(const Point& v0, const Point& v1, const Point& v2,
                                        const Point& v3) const noexcept;
        [[nodiscard]]
        static unsigned int fixAmountStep(unsigned int amountStep) noexcept;

        [[nodiscard]]
        static unsigned int genSize(unsigned int amountStep) noexcept;

    private:
        Polygon _initPolygon;
        unsigned int _amountSteps;
        double _ratio;
        std::shared_ptr<Renderer::Image> _img;
        std::shared_ptr<Renderer::Sprite> _sprite;
    };
}
