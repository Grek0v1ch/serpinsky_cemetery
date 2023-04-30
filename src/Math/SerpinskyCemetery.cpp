#include "SerpinskyCemetery.h"

#include <memory>
#include <iostream>
#include <cmath>

#include "../ResourceManager/ResourceManager.h"
#include "../Renderer/Texture2D.h"
#include "../Renderer/Sprite.h"
#include "../Renderer/Image.h"

#define IMAGE_SIZE 729

namespace Math {
    using namespace Renderer;

    SerpinskyCemetery::SerpinskyCemetery(const Polygon& userPolygon, const int amountSteps,
                                         const double ratio)
    : _viewPort{{0, 0}, {1, 1}}
    , _initPolygon(userPolygon)
    , _amountSteps(fixAmountStep(amountSteps))
    , _ratio(std::abs(ratio))
    , _img(std::make_shared<Image>(IMAGE_SIZE, IMAGE_SIZE))
    , _sprite(nullptr) {
        makeFractal();
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

    void SerpinskyCemetery::setStep(unsigned int amountSteps) {
        _amountSteps = fixAmountStep(amountSteps);
        _img = std::make_shared<Image>(IMAGE_SIZE, IMAGE_SIZE);
        makeFractal();
    }

    void SerpinskyCemetery::setRatio(double ratio) {
        _ratio = std::abs(ratio);
        makeFractal();
    }

    void SerpinskyCemetery::setInitPolygon(const Polygon& initPolygon) {
        _initPolygon = initPolygon;
        _img->clear();
        makeFractal();
    }

    void SerpinskyCemetery::setViewPort(const Renderer::ViewPort& viewPort) {
        _viewPort = viewPort;
        _img->clear();
        makeFractal();
    }

    void SerpinskyCemetery::initSprite() noexcept {
        std::shared_ptr<Texture2D> texture = std::make_shared<Texture2D>(_img->width(),
                                                                         _img->height(),
                                                                         _img->data());
        const ResourceManager& res = ResourceManager::instance();
        _sprite = std::make_shared<Sprite>(texture, res.getShaderProgram("SpriteShader"),
                                           glm::vec2{0.f},
                                           (_sprite ? _sprite->size() : glm::vec2(IMAGE_SIZE)));
    }

    void SerpinskyCemetery::makeFractal() {
        Polygon startPolygon = _initPolygon;
        startPolygon.scale(IMAGE_SIZE);
        fillPolygon(startPolygon, Color::BLACK);
        genFractal(startPolygon, _amountSteps);
        initSprite();
    }

    void SerpinskyCemetery::genFractal(const Polygon& polygon, int currSteps) {
        if (currSteps <= 0) {
            return;
        }
        auto ABPoints = getTwoPointsinRatio(polygon.a, polygon.b, _ratio);
        auto BCPoints = getTwoPointsinRatio(polygon.b, polygon.c, _ratio);
        auto CDPoints = getTwoPointsinRatio(polygon.c, polygon.d, _ratio);
        auto DAPoints = getTwoPointsinRatio(polygon.d, polygon.a, _ratio);

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

    void SerpinskyCemetery::fillPolygon(const Polygon& o, Color color) {
        fillTriangle(o.a, o.b, o.c, color);
        fillTriangle(o.c, o.d, o.a, color);
    }

    void SerpinskyCemetery::scaleTriangle(Vector& v0, Vector& v1, Vector& v2) const noexcept {
        auto crop = [](Vector&& o) -> Vector {
            o.x = o.x < 0 ? 0 : o.x;
            o.y = o.y < 0 ? 0 : o.y;
            return o;
        };
        auto temp = _viewPort.leftBottom;
        temp.scale(_img->width(), _img->height());
        v0 = crop(v0 - temp);
        v1 = crop(v1 - temp);
        v2 = crop(v2 - temp);
        auto diff = _viewPort.rightTop - _viewPort.leftBottom;
        double scaleCoefX = diff.x > 0 ? 1. / diff.x : 0.;
        double scaleCoefY = diff.y > 0 ? 1. / diff.y : 0.;
        v0.scale(scaleCoefX, scaleCoefY);
        v1.scale(scaleCoefX, scaleCoefY);
        v2.scale(scaleCoefX, scaleCoefY);
    }

    void SerpinskyCemetery::fillTriangle(Vector v0, Vector v1, Vector v2, Color color) {
        scaleTriangle(v0, v1, v2);
        int minX = ceil(std::min(std::min(v0.x, v1.x), v2.x));
        int maxX = ceil(std::max(std::max(v0.x, v1.x), v2.x));
        int minY = ceil(std::min(std::min(v0.y, v1.y), v2.y));
        int maxY = ceil(std::max(std::max(v0.y, v1.y), v2.y));
        for (unsigned int x = minX; x <= maxX and x < _img->width(); ++x) {
            for (unsigned int y = minY; y <= maxY and y < _img->height(); ++y) {
                Vector p = {static_cast<double>(x), static_cast<double>(y)};
                double e10 = edge(v1, v0, p);
                double e21 = edge(v2, v1, p);
                double e02 = edge(v0, v2, p);
                if (e10 >= 0 and e21 >= 0 and e02 >= 0) {
                    _img->setColor(x, y, color);
                }
            }
        }
    }

    double SerpinskyCemetery::edge(Vector v0, Vector v1, Vector p) const noexcept {
            Vector a = p - v0;
            Vector b = v1 - v0;
            return a.y * b.x - a.x * b.y;
    }

    Vector SerpinskyCemetery::divSegmentInRatio(const Vector& v0, const Vector& v1,
                                               const double ratio) const noexcept {
        double x = (v0.x + ratio * v1.x) / (1 + ratio);
        double y = (v0.y + ratio * v1.y) / (1 + ratio);
        return {x, y};
    }

    std::pair<Vector, Vector> SerpinskyCemetery::getTwoPointsinRatio(
            const Vector& v0,
            const Vector& v1,
            const double ratio) const noexcept {
        Vector p0 = divSegmentInRatio(v0, v1, ratio / (1 + ratio));
        Vector p1 = divSegmentInRatio(v0, v1, (ratio + 1) / ratio);
        return {p0, p1};
    }

    Vector SerpinskyCemetery::getIntersectionTwoSegment(const Vector& v0, const Vector& v1,
                                                       const Vector& v2,
                                                       const Vector& v3) const noexcept {
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
        auto diff = _viewPort.rightTop - _viewPort.leftBottom;
        double scaleCoefX = diff.x > 0 ? 1. / diff.x : 0.;
        double scaleCoefY = diff.y > 0 ? 1. / diff.y : 0.;
        double scaleCoef = std::max(scaleCoefX, scaleCoefY);
        int optimalAmountStep = static_cast<int>(std::log(IMAGE_SIZE * scaleCoef) / std::log(3));
        if (optimalAmountStep >= amountStep) {
            return amountStep;
            std::cout << amountStep << std::endl;
        } else {
            return optimalAmountStep;
            std::cout << optimalAmountStep << std::endl;
        }
    }

    unsigned int SerpinskyCemetery::genSize(const unsigned int amountStep) noexcept {
        return 729;
    }
}