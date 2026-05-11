#include <cstdlib>
#include <vector>

struct Chunk {
	/** If free, next contains the address of the next chunk **/
	Chunk* next;
};

class MemoryPool {
	public: 
		MemoryPool(size_t chunkSize, size_t chunksInBlock)
			: mChunksInBlock(chunksInBlock) {
				size_t actualChunkSize = std::max(chunkSize, sizeof(Chunk));

				// Bit alignment
				mChunkSize = (actualChunkSize + 7) & ~7;
			}

		~MemoryPool() {
			for(void* block: mBlocks) {
				free(block);
			}
		}

		void* allocate() {
			if (allo_ptr == nullptr) {
				allo_ptr = allocateBlock();
			}

			Chunk* freeChunk = allo_ptr;
			allo_ptr = allo_ptr->next;

			return freeChunk;
		}

		void deallocate(void* chunk) {
			if (!chunk) return;

			static_cast<Chunk*>(chunk)->next = allo_ptr;
			allo_ptr = static_cast<Chunk*>(chunk);
		}

	private:
		size_t mChunksInBlock;

		size_t mChunkSize;

		Chunk* allo_ptr = nullptr;

		std::vector<void*> mBlocks;

		Chunk* allocateBlock() {

			size_t totalSize = mChunksInBlock * mChunkSize;
			void* blockBegin = malloc(totalSize);
			mBlocks.push_back(blockBegin);

			Chunk* firstChunk = static_cast<Chunk*>(blockBegin);
			Chunk* current = firstChunk;

			for (int i = 0; i < mChunksInBlock - 1; ++i) {
				current->next = reinterpret_cast<Chunk*>(reinterpret_cast<char*>(current) + mChunkSize);
				current = current->next;
			}

			current->next = nullptr;

			return firstChunk;
		}
};
