
#include "manager.h"
#include <cassert>
#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <exception>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <stdio.h>
#include <stdlib.h>
#include <vector>

constexpr std::size_t BUFFER_SIZE = 1024 * 1024 * 8; // 1MB
alignas(std::max_align_t) char buffer[BUFFER_SIZE];
constexpr int N = 100000;

struct PoolDeleter {
  void operator()(void *ptr) const {
    mm::deallocate(ptr); // Return block to your pool
  }
};

struct PoolDeleter_malloc {
  void operator()(void *ptr) const {
    free(ptr); // Return block to your pool
  }
};

int main() {
  mm::init_pool(buffer, BUFFER_SIZE);

  std::vector<void *> ptrs(N);

  std::vector<void *> ptrs_malloc(N);

  auto start = std::chrono::high_resolution_clock::now();
  try {
    for (int i = 0; i < N; ++i) {
      ptrs[i] = mm::allocate(32);
      assert(ptrs[i] != nullptr);
      //     if (i == N / 2)
      //        throw std::runtime_error("simulated exception");
    }
    std::cout << "free block after allocation1: " << mm::bytes_free()
              << std::endl;

    for (int i = 0; i < N; i += 2) {
      mm::deallocate(ptrs[i]);
      ptrs[i] = nullptr;
    }

    std::cout << "free block after de-allocation: " << mm::bytes_free()
              << std::endl;

    std::cout << "largest free block after de-allocation: "
              << mm::largest_free_block() << std::endl;

    std::vector<void *> nptrs(N);
    for (int i = 0; i < N; i += 2) {
      nptrs[i] = mm::allocate(32);
      assert(nptrs[i] != nullptr);
    }

    std::cout << "free block after allocation2: " << mm::bytes_free()
              << std::endl;

    std::cout << "largest free block after allocation2: "
              << mm::largest_free_block() << std::endl;

  } catch (const std::exception &e) {
    printf("caugh exception %s\n", e.what());
  }
  auto end = std::chrono::high_resolution_clock::now();

  /* auto check = mm::bytes_free();
  auto bytes_used = mm::bytes_used();
  printf("bytes free: %zu\n", check);

  printf("bytes used: %zu\n", bytes_used);
*/
  double custom_time = std::chrono::duration<double>(end - start).count();
  printf("Custom allocator: %.6f seconds\n", custom_time);

  auto start1 = std::chrono::high_resolution_clock::now();
  try {
    for (int i = 0; i < N; ++i) {
      ptrs[i] = malloc(32);
      assert(ptrs[i] != nullptr);
    }
    std::cout << "malloc allocation 1 complete" << std::endl;

    for (int i = 0; i < N; i += 2) {
      free(ptrs[i]);
      ptrs[i] = nullptr;
    }

    std::cout << "malloc de-allocation  complete" << std::endl;
    std::vector<void *> nptrs(N);
    for (int i = 0; i < N; i += 2) {
      nptrs[i] = malloc(32);
      assert(nptrs[i] != nullptr);
    }

    std::cout << "malloc allocation 2 complete" << std::endl;

  } catch (const std::exception &e) {
    printf("caugh exception %s\n", e.what());
  }
  auto end1 = std::chrono::high_resolution_clock::now();

  double malloc_time = std::chrono::duration<double>(end1 - start1).count();
  printf("malloc: %.6f seconds\n", malloc_time);

  return 0;
}
