#include "manager.h"
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <cstring>

namespace {
using byte = unsigned char;

static void *pool_start = nullptr;
static void *pool_end = nullptr;

struct alignas(std::max_align_t) Block {
  std::size_t size;
  bool free;
  Block *next_free;
  Block *prev_free;
};

static Block *free_list_ptr = nullptr;

inline std::size_t align_up(std::size_t n, std::size_t a) {
  return ((n + a - 1) & ~(a - 1));
}

inline void *payload(Block *b) {
  return reinterpret_cast<unsigned char *>(b) + sizeof(Block);
}

inline Block *header_from_payload(void *p) {
  return reinterpret_cast<Block *>(reinterpret_cast<unsigned char *>(p) -
                                   sizeof(Block));
}

inline Block *next_block(Block *b) {
  return reinterpret_cast<Block *>(reinterpret_cast<unsigned char *>(b) +
                                   b->size);
}
inline Block *prev_block(Block *b) {
  Block *cur = reinterpret_cast<Block *>(pool_start);
  Block *prev = nullptr;
  while (cur && cur < b) {
    prev = cur;
    cur = next_block(cur);
  }
  return prev;
}

inline void freelist_insert_front(Block *b) {

  b->next_free = free_list_ptr;
  b->prev_free = nullptr;

  if (free_list_ptr != nullptr) {
    free_list_ptr->prev_free = b;
  }
  free_list_ptr = b;
}

inline void freelist_remove(Block *b) {
  if (b->prev_free != nullptr) {

    b->prev_free->next_free = b->next_free;
  } else {

    free_list_ptr = b->next_free;
  }
  if (b->next_free != nullptr) {
    b->next_free->prev_free = b->prev_free;
  }
  b->next_free = nullptr;
  b->prev_free = nullptr;
}

inline Block *freelist_find_block(std::size_t needed_size) {
  Block *curr = free_list_ptr;

  std::size_t aligned_needed_size =
      align_up(needed_size, alignof(std::max_align_t));
  std::size_t candidate = sizeof(Block) + aligned_needed_size;
  needed_size = align_up(candidate, alignof(std::max_align_t));

  while (curr) {
    if (curr->size >= needed_size) {

      freelist_remove(curr);
      std::size_t leftover_size;

      leftover_size = curr->size - needed_size;
      if (leftover_size >=
          align_up(sizeof(Block) + 1, alignof(std::max_align_t))) {

        Block *new_block = reinterpret_cast<Block *>(
            reinterpret_cast<unsigned char *>(curr) + needed_size);

        new_block->size = leftover_size;
        new_block->free = true;
        freelist_insert_front(new_block);
        curr->size = needed_size;
      }
      curr->free = false;
      return curr;
    }
    curr = curr->next_free;
  }
  return nullptr;
}

inline void coalesce(Block *header) {

  Block *next = next_block(header);
  if (next != nullptr && next->free) {
    header->size += next->size;
    freelist_remove(next);
  }
  Block *prev = prev_block(header);
  if (prev != nullptr && prev->free) {

    freelist_remove(header);
    prev->size += header->size;
    header = prev;
  }
}

} // namespace

namespace mm {

void init_pool(void *buffer, std::size_t size) noexcept {
  std::size_t alignment = alignof(std::max_align_t);
  uintptr_t addr = reinterpret_cast<uintptr_t>(buffer);
  uintptr_t aligned_addr = (addr + alignment - 1) & ~(alignment - 1);
  std::size_t usable_size = size - (aligned_addr - addr);
  pool_start = reinterpret_cast<void *>(aligned_addr);
  pool_end = reinterpret_cast<void *>(aligned_addr + usable_size);
  Block *first_block = reinterpret_cast<Block *>(aligned_addr);
  first_block->size = usable_size;
  first_block->free = true;
  first_block->next_free = nullptr;
  first_block->prev_free = nullptr;
  free_list_ptr = first_block;
}

void *allocate(std::size_t size, std::size_t) noexcept {
  Block *found = freelist_find_block(size);
  if (found != nullptr) {
    return payload(found);
  }
  return nullptr;
}

void deallocate(void *ptr) noexcept {
  Block *header = header_from_payload(ptr);
  header->free = true;
  freelist_insert_front(header);
  coalesce(header);
}

//  STATISTIC FUNCTIONS:

std::size_t bytes_used() noexcept {

  std::size_t total_used = 0;
  Block *curr = reinterpret_cast<Block *>(pool_start);
  while (reinterpret_cast<void *>(curr) < pool_end) {
    if (!curr->free) {
      total_used += curr->size;
    }
    curr = next_block(curr);
  }
  return total_used;
}

std::size_t bytes_free() noexcept {

  std::size_t total_used = 0;
  Block *curr = reinterpret_cast<Block *>(pool_start);
  while (reinterpret_cast<void *>(curr) < pool_end) {
    if (curr->free) {
      total_used += curr->size;
    }
    curr = next_block(curr);
  }
  return total_used;
}

std::size_t largest_free_block() noexcept {
  Block *curr = free_list_ptr;
  std::size_t max_size = 0;
  while (curr) {
    if (curr->size > max_size) {
      max_size = curr->size;
    }

    curr = curr->next_free;
  }
  return max_size;
}

} // namespace mm
