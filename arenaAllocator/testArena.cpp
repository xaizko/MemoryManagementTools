#include <iostream>
#include "Arena.cpp"

struct Point2D {
	double x, y;
	Point2D(double x, double y) : x(x), y(y) {}
	~Point2D() { std::cout << "2D Point Destroyed\n"; }
};

struct Point3D {
	Point2D  point2d;
	double z;
	Point3D(Point2D point2d, double z) : point2d(point2d), z(z) {}
	~Point3D() { std::cout << "3D Point Destoryed\n"; }
};

int main() {
	
}
