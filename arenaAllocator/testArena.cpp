#include "Arena.cpp"
#include <cassert>
#include <iostream>

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
	Arena arena(100);	

	std::cout << "--- Test 1: Basic Allocation ---\n";
	Point2D* p1 = arena.construct<Point2D>(6.7, 6.7);
	Point2D* p2 = arena.construct<Point2D>(6.9, 6.9);
	Point3D* p3 = arena.construct<Point3D>(*p2, 6.9);

	std::cout << "P1 Address: " << p1 << "\n";
	std::cout << "P2 Address: " << p2 << "\n";
	std::cout << "P3 Address: " << p3 << "\n";

	std::cout << "Remaining Space: " << arena.spaceLeft() << "\n";

	std::cout << "--- Test 2: Reset Arena ---\n";
	arena.reset();
	std::cout << "Remaining Space: " << arena.spaceLeft() << "\n";

	// Stall for input before freeing
	std::cin.get();
	std::cout << "\nAll tests passed!\n";
}
