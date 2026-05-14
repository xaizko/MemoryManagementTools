# Memory Management Experiments

This repository contains two small C++ memory-management exercises:

- `arenaAllocator/` implements a simple arena allocator.
- `memoryPool/` implements a fixed-size memory pool.

Both implementations were written as a learning challenge to explore how manual allocation strategies work under the hood. They are intentionally small and practical for experimentation, but they are not polished or production-ready.

## What this repository is for

The goal of the project is to demonstrate two classic allocation strategies that sit below normal `new` and `delete` usage:

- An arena allocator that hands out memory in sequence from one or more large blocks.
- A memory pool that manages equally sized chunks and reuses freed chunks.

The code is useful for understanding the basic mechanics of allocation, alignment, block growth, and reuse. It is not meant to be a general-purpose allocator library.

## Repository layout

- `arenaAllocator/Arena.cpp` - arena allocator implementation.
- `arenaAllocator/testArena.cpp` - small test program for the arena allocator.
- `memoryPool/MemoryPool.cpp` - memory pool implementation.
- `memoryPool/testMemoryPool.cpp` - small test program for the memory pool.

## Arena allocator

The arena allocator provides a lightweight way to allocate many objects from a shared block of memory.

Main behavior:

- `Arena(size_t blockSize)` creates the first backing block.
- `alloc(size_t size, size_t alignment)` returns aligned memory from the current block and allocates a larger block if the current one is full.
- `construct<T>(Args&&...)` allocates raw memory and placement-news an object into it.
- `reset()` rewinds the arena back to the first block so memory can be reused.
- `getRemainingSize()` reports how much space is left in the active block.

This implementation is best understood as a fast bump allocator. It does not track individual allocations for later freeing.

## Memory pool

The memory pool manages a free list of same-sized chunks.

Main behavior:

- `MemoryPool(size_t chunkSize, size_t chunksInBlock)` sets the size of each chunk and how many chunks are placed in one backing block.
- `allocate()` returns a chunk from the free list, creating a new block when needed.
- `deallocate(void* chunk)` returns a chunk to the free list so it can be reused.
- The destructor frees the raw blocks that were allocated.

This is a simple fixed-size allocator that is useful when many objects of the same size are created and destroyed repeatedly.

## Test programs

The test files are small console programs that demonstrate the allocators in action:

- `arenaAllocator/testArena.cpp` constructs a few sample objects, forces the arena to grow, and then calls `reset()`.
- `memoryPool/testMemoryPool.cpp` allocates chunks, constructs objects with placement new, returns a chunk to the pool, and allocates again to show reuse.

## Important limitations

This code is intentionally basic and should not be treated as a professional allocator implementation.

- It uses raw `malloc` and `free` directly.
- It does not provide full exception safety.
- It does not automatically destroy objects stored in the arena.
- It assumes a very simple allocation model.
- It is meant for learning and experimentation, not for real production systems.

## Summary

If you want to understand how custom allocators work, this repository is a good study project. If you need memory management for real software, use a mature allocator or standard container approach instead of this code.
