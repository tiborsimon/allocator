#ifndef ALLOC_H
#define ALLOC_H

typedef struct Allocator Allocator;

struct Allocator {
  void* blocks[];
  void* pointers[];
  unsigned int block_count;
}

Allocator* initAllocator();
Allocator* initAllocator(int size);

void* alloc(unsigned int count, unsigned int size, Allocator* Allocator);

int destroyAllocator(Allocator* allocator);

#endif /* ALLOC_H */

