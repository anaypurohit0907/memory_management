#pragma once

#ifndef MEMORY_MANAGEMT_MANAGER_H
#define MEMORY_MANAGEMT_MANAGER_H

#include <cstddef>

namespace mm {

void init_pool(void *buffer, std::size_t size) noexcept;

void *allocate(std::size_t size,
               std::size_t alignment = alignof(std::max_align_t)) noexcept;

void deallocate(void *ptr) noexcept;

std::size_t bytes_used() noexcept;
std::size_t bytes_free() noexcept;
std::size_t largest_free_block() noexcept;

} // namespace mm

extern "C" {

void *mm_malloc(std::size_t size) noexcept; // delegates to mm::allocate
void mm_free(void *ptr) noexcept;           // delegates to mm::deallocate
}

#endif
