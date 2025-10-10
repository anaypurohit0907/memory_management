
#include "manager.h"
#include <cassert>
#include <chrono>
#include <cstdio>
#include <stdio.h>
#include <stdlib.h>
#include <vector>

constexpr std::size_t BUFFER_SIZE = 1024 * 1024 * 8; // 1MB
alignas(std::max_align_t) char buffer[BUFFER_SIZE];
constexpr int N = 100000;

int main() {
  mm::init_pool(buffer, BUFFER_SIZE);
  std::vector<void *> ptrs(N);

  auto start = std::chrono::high_resolution_clock::now();
  for (int i = 0; i < N; ++i) {
    ptrs[i] = mm::allocate(32);
    assert(ptrs[i] != nullptr);
  }
  for (int i = 0; i < N; ++i) {
    mm::deallocate(ptrs[i]);
  }
  auto end = std::chrono::high_resolution_clock::now();
  double custom_time = std::chrono::duration<double>(end - start).count();
  printf("Custom allocator: %.6f seconds\n", custom_time);

  start = std::chrono::high_resolution_clock::now();
  for (int i = 0; i < N; ++i) {
    ptrs[i] = malloc(32);
    assert(ptrs[i] != nullptr);
  }
  for (int i = 0; i < N; ++i) {
    free(ptrs[i]);
  }
  end = std::chrono::high_resolution_clock::now();
  double malloc_time = std::chrono::duration<double>(end - start).count();
  printf("malloc/free:      %.6f seconds\n", malloc_time);

  return 0;
}
