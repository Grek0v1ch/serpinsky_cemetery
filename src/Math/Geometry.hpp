#pragma once

namespace Math {
	struct Vector {
	    double x;
	    double y;
	};

	struct Point {
	    double x;
	    double y;

	    Vector operator-(const Point& rhs) {
	        return {x - rhs.x, y - rhs.y};
	    }
	};

	struct Polygon {
	    Point a;
	    Point b;
	    Point c;
	    Point d;
	};
}
