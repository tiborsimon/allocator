#include <CppUTest/TestHarness.h>

#include "allocator.h"

TEST_GROUP(AllocatorBasicCases) {
};

TEST(AllocatorBasicCases, init_and_free_with_default_size) {
  Allocator* allocator;
  allocator = initAllocator();

  CHECK(allocator);
  CHECK_EQUAL(1024, allocator->block_size);
  CHECK(allocator->node);

  POINTERS_EQUAL(NULL, allocator->node->next);
  CHECK(allocator->node->block);
  POINTERS_EQUAL(allocator->node->pointer, allocator->node->block);
  CHECK_EQUAL(1024, allocator->node->size);

  freeAllocator(allocator);
}

TEST(AllocatorBasicCases, init_and_free_with_custom_size) {
  Allocator* allocator;
  allocator = initAllocator(42);

  CHECK(allocator);
  CHECK_EQUAL(42, allocator->block_size);
  CHECK(allocator->node);

  POINTERS_EQUAL(NULL, allocator->node->next);
  CHECK(allocator->node->block);
  POINTERS_EQUAL(allocator->node->pointer, allocator->node->block);
  CHECK_EQUAL(42, allocator->node->size);

  freeAllocator(allocator);
}

TEST(AllocatorBasicCases, allocating__no_block_overrun) {
  Allocator* allocator;
  uint8_t* dummy = NULL;

  allocator = initAllocator(2);

  dummy = (uint8_t*)alloc(1, 1, allocator);
  dummy[0] = 42;

  CHECK(dummy);
  POINTERS_EQUAL(allocator->node->block, dummy);
  POINTERS_EQUAL(allocator->node->pointer, dummy+1);
  CHECK_EQUAL(1, allocator->node->size);


  freeAllocator(allocator);
}

TEST(AllocatorBasicCases, allocating__no_block_overrun_but_first_block_is_full) {
  Allocator* allocator;
  uint8_t* dummy = NULL;

  allocator = initAllocator(2);

  dummy = (uint8_t*)alloc(2, 1, allocator);
  dummy[0] = 42;
  dummy[1] = 43;

  CHECK(dummy);
  POINTERS_EQUAL(allocator->node->block, dummy);
  POINTERS_EQUAL(allocator->node->pointer, dummy+2);
  CHECK_EQUAL(0, allocator->node->size);

  freeAllocator(allocator);
}

TEST(AllocatorBasicCases, allocating__bigger_request_than_block_size__returns_null) {
  Allocator* allocator;
  uint8_t* dummy = NULL;

  allocator = initAllocator(2);
  dummy = (uint8_t*)alloc(3, 1, allocator);

  POINTERS_EQUAL(NULL, dummy);

  freeAllocator(allocator);
}

TEST(AllocatorBasicCases, allocating__new_block_needed) {
  Allocator* allocator;
  uint8_t* dummy = NULL;

  allocator = initAllocator(2);

  dummy = (uint8_t*)alloc(2, 1, allocator);
  dummy = (uint8_t*)alloc(1, 1, allocator);
  dummy[0] = 42;

  CHECK(dummy);
  POINTERS_EQUAL(allocator->node->next->block, dummy);
  POINTERS_EQUAL(allocator->node->next->pointer, dummy+1);
  CHECK_EQUAL(1, allocator->node->next->size);
  POINTERS_EQUAL(NULL, allocator->node->next->next);

  freeAllocator(allocator);
}

TEST(AllocatorBasicCases, allocating__two_new_block_needed) {
  Allocator* allocator;
  uint8_t* dummy = NULL;

  allocator = initAllocator(2);

  dummy = (uint8_t*)alloc(2, 1, allocator);
  dummy[0] = 42;
  dummy[1] = 42;

  dummy = (uint8_t*)alloc(2, 1, allocator);
  dummy[0] = 42;
  dummy[1] = 42;

  dummy = (uint8_t*)alloc(1, 1, allocator);
  dummy[0] = 42;

  CHECK(dummy);
  POINTERS_EQUAL(allocator->node->next->next->block, dummy);
  POINTERS_EQUAL(allocator->node->next->next->pointer, dummy+1);
  CHECK_EQUAL(1, allocator->node->next->next->size);
  POINTERS_EQUAL(NULL, allocator->node->next->next->next);

  freeAllocator(allocator);
}

