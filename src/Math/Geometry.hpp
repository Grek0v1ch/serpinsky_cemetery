#pragma once

namespace Math {
	struct Vector {
	    double x;
	    double y;
	};

	struct Point {
	    double x;
	    double y;

	    Vector operator-(const Point& rhs) const noexcept;

	    void scaleX(double scaleCoef) noexcept;
	    void scaleY(double scaleCoef) noexcept;
	    void scale(double scaleCoefX, double scaleCoefY) noexcept;
	    void scale(double scaleCoef) noexcept;
	};

	struct Polygon {
	    Point a;
	    Point b;
	    Point c;
	    Point d;

	    void scaleX(double scaleCoef) noexcept;
	    void scaleY(double scaleCoef) noexcept;
	    void scale(double scaleCoefX, double scaleCoefY) noexcept;
	    void scale(double scaleCoef) noexcept;
	};
}
