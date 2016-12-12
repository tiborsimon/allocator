#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#include <stdint.h>

typedef struct Allocator Allocator;
typedef struct AllocatorNode AllocatorNode;

struct AllocatorNode {
  AllocatorNode* next;
  uint32_t size;
  uint8_t* block;
  uint8_t* pointer;
};

struct Allocator {
  uint32_t block_size;
  AllocatorNode* node;
};

Allocator* initAllocator();
Allocator* initAllocator(uint32_t size);
uint8_t freeAllocator(Allocator* allocator);

void* alloc(uint32_t count, uint32_t size, Allocator* Allocator);


#endif /* ALLOCATOR_H */

