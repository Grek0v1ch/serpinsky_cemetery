#include "Geometry.h"

namespace Math {
	Vector Vector::operator-(const Vector& rhs) const noexcept {
        return {x - rhs.x, y - rhs.y};
    }

    Vector Vector::operator+(const Vector& rhs) const noexcept {
    	return {x + rhs.x, y + rhs.y};
    }

    void Vector::scaleX(double scaleCoef) noexcept {
    	x *= scaleCoef;
    }

    void Vector::scaleY(double scaleCoef) noexcept {
    	y *= scaleCoef;
    }

    void Vector::scale(double scaleCoefX, double scaleCoefY) noexcept {
    	scaleX(scaleCoefX);
    	scaleY(scaleCoefY);
    }

    void Vector::scale(double scaleCoef) noexcept {
    	scaleX(scaleCoef);
    	scaleY(scaleCoef);
    }

    void Polygon::scaleX(double scaleCoef) noexcept {
    	a.scaleX(scaleCoef);
    	b.scaleX(scaleCoef);
    	c.scaleX(scaleCoef);
    	d.scaleX(scaleCoef);
    }

    void Polygon::scaleY(double scaleCoef) noexcept {
    	a.scaleY(scaleCoef);
    	b.scaleY(scaleCoef);
    	c.scaleY(scaleCoef);
    	d.scaleY(scaleCoef);
    }

    void Polygon::scale(double scaleCoefX, double scaleCoefY) noexcept {
    	a.scale(scaleCoefX, scaleCoefY);
    	b.scale(scaleCoefX, scaleCoefY);
    	c.scale(scaleCoefX, scaleCoefY);
    	d.scale(scaleCoefX, scaleCoefY);
    }

    void Polygon::scale(double scaleCoef) noexcept {
    	a.scale(scaleCoef);
    	b.scale(scaleCoef);
    	c.scale(scaleCoef);
    	d.scale(scaleCoef);
    }
}