#include <iostream>
#include <string>
#include <cassert>
#include "MemoryPool.cpp"

struct TestObject {
	int x;
	std::string s;
	TestObject(int x, std::string s) : x(x), s(s) {}
	~TestObject() { std::cout << "Object Destroyed\n"; }
};

int main() {
	// Creates a pool for 3 TestObjects
	MemoryPool pool(sizeof(TestObject), 2);

	std::cout << "--- Test 1: Basic Allocation ---\n";
	void* addr1 = pool.allocate();
	void* addr2 = pool.allocate();

	TestObject* o1 = new (addr1) TestObject(1, "Hello ");
	TestObject* o2 = new (addr2) TestObject(2, "World!");

	std::cout << "O1 Address: " << addr1 << "\n";
	std::cout << "O2 Address: " << addr2 << "\n";

	std::cout << "--- Test 2: Deallocation & Reuse ---\n";
	o1->~TestObject();
	pool.deallocate(addr1);

	void* addr3 = pool.allocate();
	std::cout << "O3 Address: " << addr2 << "\n";

	std::cout << "--- Test 3: Growth ---\n";
	void* addr4 = pool.allocate();
	std::cout << "O4 (New Block) Address: " << addr4 << "\n";
	assert(addr4 != nullptr);

	std::cout << "\nAll tests passed!\n";

	return 0;
}
