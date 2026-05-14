#include <iostream>
#include <memory>

class Arena {
	public:
		explicit Arena(size_t blockSize) : m_defaultBlockSize(blockSize) {
			m_firstBlock = allocateNewBlock(m_defaultBlockSize);
		}

		~Arena() {
			Block* current = m_currentBlock;
			while (current) {
				Block* next = current->next;
				free(current->ptr);
				free(current);
				current = next;
			}

			std::cout << "Arena Freed\n";
		}

		void* alloc(size_t size, size_t alignment) {
			size_t spaceInCurrentBlock = m_blockEnd - m_offsetPtr;
			void* ptr = m_offsetPtr;

			// Align and fit in current block
			// If fail then allocate new block
			if (std::align(alignment, size, ptr, spaceInCurrentBlock)) {
				m_offsetPtr = static_cast<char*>(ptr) + size;
				return ptr;
			}

			size_t nextBlockSize = std::max(size + alignment, m_defaultBlockSize);
			allocateNewBlock(nextBlockSize);

			ptr = m_offsetPtr;
			spaceInCurrentBlock = m_blockEnd - m_offsetPtr;

			if (std::align(alignment, size, ptr, spaceInCurrentBlock)) {
				m_offsetPtr = static_cast<char*>(ptr) + size;
				return ptr;
			}

			return nullptr;
		}

		void reset() {
			m_currentBlock = m_firstBlock;
			m_offsetPtr = static_cast<char*>(m_currentBlock->ptr);
			m_blockEnd = m_offsetPtr + m_currentBlock->size;
		}

		template<typename T, typename... Args>
		T* construct(Args&&... args) {
			void* mem = this->alloc(sizeof(T), alignof(T));
			return new (mem) T(std::forward<Args>(args)...);
		}

		size_t getRemainingSize() {
			return m_blockEnd - m_offsetPtr; 
		}

	private:
		struct Block {
			void* ptr;
			size_t size;
			Block* next;
		};

		Block* m_firstBlock = nullptr;
		Block* m_currentBlock = nullptr;
		char* m_offsetPtr = nullptr;
		char* m_blockEnd = nullptr;

		size_t m_defaultBlockSize;

		Block* allocateNewBlock(size_t size) {
			void* rawMem = malloc(size);
			if (!rawMem) throw std::bad_alloc();

			// Create new block
			Block* newBlock = (Block*)malloc(sizeof(Block));
			newBlock->ptr = rawMem;
			newBlock->size = size;

			// New block becomes head of chain
			newBlock->next = m_currentBlock;
			m_currentBlock = newBlock;

			// Updating arena
			m_offsetPtr = static_cast<char*>(rawMem);
			m_blockEnd = m_offsetPtr + size;

			return newBlock;
		}
};
