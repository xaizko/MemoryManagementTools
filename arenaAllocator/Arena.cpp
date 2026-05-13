#include <cstdlib>
#include <cstddef>
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
		};

		void* alloc(size_t size, size_t alignment) {
			void* ptr = offsetPtr;

			if (std::align(alignment, size, ptr, space)) {
				offsetPtr = static_cast<char*>(ptr) + size;
				space -= size;

				return ptr;
			}
			
			// Invalid, Fix later
			return ptr;
		};

		void reset() {
			offsetPtr = basePtr;
		};

		template<typename T, typename... Args>
		T* construct(Args&&... args) {
			void* mem = this->alloc(sizeof(T), alignof(T));
			return new (mem) T(std::forward<args>(args)...);
		}

	private:
		std::stack<void*> ptrs;
		void* basePtr;
		void* blockEnd; 
		void* offsetPtr;
		size_t space; 
};
