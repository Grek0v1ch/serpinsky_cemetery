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

    SerpinskyCemetery::SerpinskyCemetery(const Polygon& userPolygon, int amountSteps = 1)
    noexcept : _amountSteps(fixAmountStep(amountSteps))
             , _img(std::make_shared<Image>(genSize(_amountSteps), genSize(_amountSteps)))
             , _sprite(nullptr) {
        double width = static_cast<double>(genSize(_amountSteps));
        double height = static_cast<double>(genSize(_amountSteps));
        _initPolygon = {{userPolygon.a.x * width, userPolygon.a.y * height},
                        {userPolygon.b.x * width, userPolygon.b.y * height},
                        {userPolygon.c.x * width, userPolygon.c.y * height},
                        {userPolygon.d.x * width, userPolygon.d.y * height}};
        fillPolygon(_initPolygon, Color::BLACK);
        genFractal(_initPolygon, _amountSteps);
        initSprite();
    }

    void SerpinskyCemetery::render() noexcept {
        if (_sprite) {
            _sprite->render();
        }
    }

    void SerpinskyCemetery::setWH(const unsigned int newWidth,
                                  const unsigned int newHeight) noexcept {
        _sprite->setSize(glm::vec2(newWidth, newHeight));
    }

    void SerpinskyCemetery::setStep(unsigned int amountSteps) noexcept {
        _amountSteps = fixAmountStep(amountSteps);
        fillPolygon(_initPolygon, Color::BLACK);
        genFractal(_initPolygon, _amountSteps);
        initSprite();
    }

    void SerpinskyCemetery::initSprite() noexcept {
        unsigned int size = genSize(_amountSteps);
        std::shared_ptr<Texture2D> texture = std::make_shared<Texture2D>(_img->width(),
                                                                         _img->height(),
                                                                         _img->data());
        const ResourceManager& res = ResourceManager::instance();
        _sprite = std::make_shared<Sprite>(texture, res.getShaderProgram("SpriteShader"),
                                           glm::vec2{0.0f},
                                           (_sprite ? _sprite->size() : glm::vec2(size)));
    }

    void SerpinskyCemetery::genFractal(const Polygon& polygon, int currSteps) noexcept {
        double ratio = 1.;
        if (currSteps <= 0) {
            return;
        }
        auto ABPoints = getTwoPointsinRatio(polygon.a, polygon.b, ratio);
        auto BCPoints = getTwoPointsinRatio(polygon.b, polygon.c, ratio);
        auto CDPoints = getTwoPointsinRatio(polygon.c, polygon.d, ratio);
        auto DAPoints = getTwoPointsinRatio(polygon.d, polygon.a, ratio);

        auto v0 = getIntersectionTwoSegment(BCPoints.first, DAPoints.second,
                                            ABPoints.second, CDPoints.first);
        auto v1 = getIntersectionTwoSegment(BCPoints.second, DAPoints.first,
                                            ABPoints.second, CDPoints.first);
        auto v3 = getIntersectionTwoSegment(BCPoints.first, DAPoints.second,
                                            ABPoints.first, CDPoints.second);
        auto v2 = getIntersectionTwoSegment(BCPoints.second, DAPoints.first,
                                            ABPoints.first, CDPoints.second);

        fillPolygon({ABPoints.first, ABPoints.second, v0, v3}, Color::WHITE);
        fillPolygon({BCPoints.first, BCPoints.second, DAPoints.first, DAPoints.second},
                     Color::WHITE);
        fillPolygon({v2, v1, CDPoints.first, CDPoints.second}, Color::WHITE);

        genFractal({ABPoints.second, polygon.b, BCPoints.first, v0}, currSteps - 1);
        genFractal({BCPoints.second, polygon.c, CDPoints.first, v1}, currSteps - 1);
        genFractal({DAPoints.first, v2, CDPoints.second, polygon.d}, currSteps - 1);
        genFractal({polygon.a, ABPoints.first, v3, DAPoints.second}, currSteps - 1);
    }

    void SerpinskyCemetery::fillPolygon(const Polygon& o, Color color) noexcept {
        fillTriangle(o.a, o.b, o.c, color);
        fillTriangle(o.c, o.d, o.a, color);
    }

    void SerpinskyCemetery::fillTriangle(Point v0, Point v1, Point v2, Color color) noexcept {
        int minX = ceil(std::min(std::min(v0.x, v1.x), v2.x));
        int maxX = ceil(std::max(std::max(v0.x, v1.x), v2.x));
        int minY = ceil(std::min(std::min(v0.y, v1.y), v2.y));
        int maxY = ceil(std::max(std::max(v0.y, v1.y), v2.y));
        for (unsigned int x = minX; x <= maxX and x < _img->width(); ++x) {
            for (unsigned int y = minY; y <= maxY and y < _img->height(); ++y) {
                Point p = {static_cast<double>(x), static_cast<double>(y)};
                double e10 = edge(v1, v0, p);
                double e21 = edge(v2, v1, p);
                double e02 = edge(v0, v2, p);
                if (e10 >= 0 and e21 >= 0 and e02 >= 0) {
                    _img->setColor(x, y, color);
                }
            }
        }
    }

    double SerpinskyCemetery::edge(Point v0, Point v1, Point p) const noexcept {
            Vector a = p - v0;
            Vector b = v1 - v0;
            return a.y * b.x - a.x * b.y;
    }

    Point SerpinskyCemetery::divSegmentInRatio(const Point& v0, const Point& v1,
                                               const double ratio) const noexcept {
        double x = (v0.x + ratio * v1.x) / (1 + ratio);
        double y = (v0.y + ratio * v1.y) / (1 + ratio);
        return {x, y};
    }

    std::pair<Point, Point> SerpinskyCemetery::getTwoPointsinRatio(
            const Point& v0,
            const Point& v1,
            const double ratio) const noexcept {
        Point p0 = divSegmentInRatio(v0, v1, ratio / (1 + ratio));
        Point p1 = divSegmentInRatio(v0, v1, (ratio + 1) / ratio);
        return {p0, p1};
    }

    Point SerpinskyCemetery::getIntersectionTwoSegment(const Point& v0, const Point& v1,
                                                       const Point& v2,
                                                       const Point& v3) const noexcept {
        double denom = (v0.x - v1.x) * (v2.y - v3.y) - (v0.y - v1.y) * (v2.x - v3.x);
        double x = (v0.x * v1.y - v0.y * v1.x) * (v2.x - v3.x)
                 - (v0.x - v1.x) * (v2.x * v3.y - v2.y * v3.x);
        double y = (v0.x * v1.y - v0.y * v1.x) * (v2.y - v3.y)
                 - (v0.y - v1.y) * (v2.x * v3.y - v2.y * v3.x);
        x /= denom;
        y /= denom;
        return {x, y};
    }

    unsigned int SerpinskyCemetery::fixAmountStep(const unsigned int amountStep) noexcept {
        if (amountStep > 8) {
            return 8;
        }
        else {
            return amountStep;
        }
    }

    unsigned int SerpinskyCemetery::genSize(const unsigned int amountStep) noexcept {
        if (amountStep <= 5) {
            return 243;
        } else {
            return static_cast<unsigned int>(pow(3, amountStep));
        }
    }
}