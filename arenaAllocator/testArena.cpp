#include "Arena.cpp"
#include <cassert>
#include <iostream>
#include <limits>

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
	int spaceNeeded = 0;
	std::cout << "Enter amount of space needed in arena: ";
	while(!(std::cin >> spaceNeeded)) {
		std::cout << "Invalid input. Enter a number: ";
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	}
	
	Arena arena(spaceNeeded);	

	std::cout << "--- Test 1: Basic Allocation ---\n";
	Point2D* p1 = arena.construct<Point2D>(6.7, 6.7);
	Point2D* p2 = arena.construct<Point2D>(6.9, 6.9);
	Point3D* p3 = arena.construct<Point3D>(*p2, 6.9);

	std::cout << "P1 Address: " << p1 << "\n";
	std::cout << "P2 Address: " << p2 << "\n";
	std::cout << "P3 Address: " << p3 << "\n";

	std::cout << "--- Test 2: Test Resizing ---\n";
	size_t initialSpace = arena.getRemainingSize();
	std::cout << "Space before forcing resize: " << initialSpace << " bytes\n";

	Point3D* p4 = arena.construct<Point3D>(Point2D(1.1, 2.2), 3.3);

	size_t newSpace = arena.getRemainingSize();
	std::cout << "P4 Address (New Block): " << p4 << "\n";
	std::cout << "Space in the NEW block: " << newSpace << " bytes\n";

	assert(p1->x == 6.7);
	assert(p4->z == 3.3);
	std::cout << "Resize successful: Old and new objects are both valid.\n";

	std::cout << "--- Test 3: Reset Arena ---\n";
	arena.reset();
	assert(arena.getRemainingSize() == spaceNeeded);

	std::cout << "\nAll tests passed!\n";
}
