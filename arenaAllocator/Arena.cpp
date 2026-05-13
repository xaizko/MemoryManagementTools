#include <cstdlib>
#include <cstddef>
#include <iostream>
#include <memory>
#include <stack>
#include <stddef.h>

class Arena {
	public:
		explicit Arena(size_t blockSize) {
			basePtr = malloc(blockSize);
			blockEnd = (char*)basePtr + blockSize;
			offsetPtr = basePtr;
			space = blockSize;
		};

		~Arena() {
			free(basePtr);
			std::cout << "Arena Freed\n";
		};

		void* alloc(size_t size, size_t alignment) {
			if (size > space) {
				return nullptr;
			}
			void* ptr = offsetPtr;

			if (std::align(alignment, size, ptr, space)) {
				offsetPtr = static_cast<char*>(ptr) + size;
				space -= size;

				return ptr;
			}

			return nullptr;
		};

		void reset() {
			offsetPtr = basePtr;
			space = (char*) blockEnd - (char*) basePtr;

		};

		template<typename T, typename... Args>
		T* construct(Args&&... args) {
			void* mem = this->alloc(sizeof(T), alignof(T));
			return new (mem) T(std::forward<Args>(args)...);
		}

		size_t spaceLeft() {
			return space;
		}

	private:
		std::stack<void*> ptrs;
		void* basePtr;
		void* blockEnd; 
		void* offsetPtr;
		size_t space; 
};
