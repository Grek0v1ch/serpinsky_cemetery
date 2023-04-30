#pragma once

namespace Math {
	struct Vector {
	    double x;
	    double y;

	    Vector operator-(const Vector& rhs) const noexcept;
	    Vector operator+(const Vector& rhs) const noexcept;

	    void scaleX(double scaleCoef) noexcept;
	    void scaleY(double scaleCoef) noexcept;
	    void scale(double scaleCoefX, double scaleCoefY) noexcept;
	    void scale(double scaleCoef) noexcept;
	};

	struct Polygon {
	    Vector a;
	    Vector b;
	    Vector c;
	    Vector d;

	    void scaleX(double scaleCoef) noexcept;
	    void scaleY(double scaleCoef) noexcept;
	    void scale(double scaleCoefX, double scaleCoefY) noexcept;
	    void scale(double scaleCoef) noexcept;
	};
}
