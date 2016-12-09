#include <stdlib.h>
#include <stdio.h>
#include "allocator.h"

Allocator* initAllocator() {
  return initAllocator(1024);
}

Allocator* initAllocator(int size) {

}

void* alloc(unsigned int count, unsigned int size, Allocator* Allocator) {

}

int destroyAllocator(Allocator* allocator) {

}
