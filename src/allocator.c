#include <stdlib.h>
#include <stdio.h>
#include "allocator.h"

static uint8_t* _allocate_from_node(AllocatorNode* node, uint32_t request) {
  uint8_t* allocated = NULL;
  allocated = node->pointer;
  node->pointer += request;
  node->size -= request;
  return allocated;
}

static AllocatorNode* _create_node(uint32_t size) {
  AllocatorNode* node = (AllocatorNode*)calloc(1, sizeof(*node));
  node->next = NULL;
  node->block = (uint8_t*)calloc(size, 1);
  node->pointer = node->block;
  node->size = size;
  return node;
}

Allocator* initAllocator() {
  return initAllocator(1024);
}

Allocator* initAllocator(uint32_t size) {
  Allocator* allocator;
  allocator = (Allocator*)calloc(1, sizeof(*allocator));
  allocator->block_size = size;
  allocator->node = _create_node(size);
  return allocator;
}

uint8_t freeAllocator(Allocator* allocator) {
  AllocatorNode* temp_node;
  AllocatorNode* node = allocator->node;
  do {
    temp_node = node->next;
    free(node->block);
    free(node);
    node = temp_node;
  } while (node != NULL);
  free(allocator);
  return 0;
}

void* alloc(uint32_t count, uint32_t size, Allocator* allocator) {
  uint8_t* allocated = NULL;
  uint32_t request = count*size;
  AllocatorNode* node;
  if (request <= allocator->block_size) {
    node = allocator->node;
    while (1) {
      if (request <= node->size) {
        allocated = _allocate_from_node(node, request);
        break;
      }
      if (!node->next) {
        break;
      }
      node = node->next;
    }
    if (!allocated) {
      node->next = _create_node(allocator->block_size);
      node = node->next;
      allocated = _allocate_from_node(node, request);
    }
  }
  return (void*)allocated;
}

