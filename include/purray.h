/*
 * purray.h - Dynamic arrays for C that purr.
 * License: The Unlicense
 *
 * Purray is a lightweight dynamic array library for C.
 * It provides a simple API for working with dynamic arrays
 * with automatic resizing and memory management.
 */
#pragma once
#ifndef PURRAY_H
#define PURRAY_H

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>

#include "internal/purray_config.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

// ============================================================================
// Constants
// ============================================================================

// 'PURR' in Little-Endian
#define PURRAY_HEADER_MAGIC						0x52525550

// ============================================================================
// Internal macros (for advanced users only)
// ============================================================================

// Convert array memory pointer to header pointer
#define PURRAY_MEM_TO_HEADER(mem)				\
	(PurrayHeader*)((uintptr_t)(mem) - sizeof(PurrayHeader))

// Convert header pointer to array memory pointer
#define PURRAY_HEADER_TO_MEM(header)			\
	(void*)((PurrayHeader*)(header) + 1)

// ============================================================================
// Types
// ============================================================================

/**
 * @brief Internal header structure for Purray arrays.
 *
 * This structure is stored immediately before the array data in memory.
 * Users should not access these fields directly.
 */
typedef struct TPurrayHeader {
	uint32_t	magic;
	void		*mem;
	size_t		elem_size;
	size_t		num_elems;
	size_t		capacity;
} PurrayHeader;

// ============================================================================
// Query functions
// ============================================================================

/**
 * @brief Get the number of elements in the array.
 * @param purray_mem Pointer to the array memory.
 * @return Number of elements, or `SIZE_MAX` if the array is invalid.
 */
size_t purray_len(void *purray_mem);

/**
 * @brief Get the size of each element in the array.
 * @param purray_mem Pointer to the array memory.
 * @return Size of each element in bytes, or `SIZE_MAX` if the array is invalid.
 */
size_t purray_elem_size(void *purray_mem);

// ============================================================================
// Memory management functions (low-level)
// ============================================================================

/**
 * @brief Allocate a new dynamic array.
 * @param capacity Initial capacity in elements.
 * @param elem_size Size of each element in bytes.
 * @return Pointer to the array memory, or `NULL` on failure.
 */
void *purray_alloc(size_t capacity, size_t elem_size);

/**
 * @brief Reallocate the array to a new capacity.
 * @param purray_mem Pointer to the array memory.
 * @param new_capacity New capacity in elements.
 * @return Pointer to the reallocated array memory, or `NULL` on failure.
 *		 If `NULL` is returned, the original array remains valid.
 */
void *purray_realloc(void *purray_mem, size_t new_capacity);

/**
 * @brief Duplicate an existing array.
 * @param purray_mem Pointer to the array memory to duplicate.
 * @return Pointer to the new array, or `NULL` on failure.
 */
void *purray_dup(const void *purray_mem);

/**
 * @brief Free the array and release all associated memory.
 * @param purray_mem Pointer to the array memory.
 */
void purray_free(void *purray_mem);

// ============================================================================
// Element manipulation functions (low-level)
// ============================================================================

/**
 * @brief Place an element at a specific position in the array.
 * @param purray_mem Pointer to the array memory.
 * @param elem Pointer to the element to place.
 * @param pos Position (index) to place the element at.
 * @return Pointer to the array memory (may change after reallocation),
 *		 or `NULL` on failure.
 */
void *purray_place(void *purray_mem, const void *elem, size_t pos);

/**
 * @brief Remove elements from the array.
 * @param purray_mem Pointer to the array memory.
 * @param elem_index Starting index of elements to remove.
 * @param count Number of elements to remove.
 * @return Pointer to the array memory, or `NULL` on failure.
 */
void *purray_remove(void *purray_mem, size_t pos, size_t count);

// ============================================================================
// High-level convenience functions (auto-handle reallocation)
// These functions take a pointer to the array pointer and update it
// automatically. If reallocation fails, the original pointer is left
// unchanged and NULL is returned.
// ============================================================================

/**
 * @brief Resize the array to a new capacity.
 * @param purray_mem_ptr Pointer to the array pointer.
 * @param new_capacity New capacity in elements.
 * @return Pointer to the resized array, or `NULL` on failure.
 *		 On failure, the original array remains valid.
 *
 * @note This function updates the pointer in *purray_mem_ptr
 *	   automatically on success.
 */
void *purray_resize(void **purray_mem_ptr, size_t new_capacity);

/**
 * @brief Insert an element at a specific position.
 * @param purray_mem_ptr Pointer to the array pointer.
 * @param elem Pointer to the element to insert.
 * @param pos Position (index) to insert at.
 * @return Pointer to the array, or `NULL` on failure.
 *		 On failure, the original array remains valid.
 *
 * @note This function updates the pointer in *purray_mem_ptr
 *	   automatically on success.
 */
void *purray_insert(void **purray_mem_ptr, const void *elem, size_t pos);

/**
 * @brief Prepend an element to the beginning of the array.
 * @param purray_mem_ptr Pointer to the array pointer.
 * @param elem Pointer to the element to prepend.
 * @return Pointer to the array, or `NULL` on failure.
 *		 On failure, the original array remains valid.
 *
 * @note This function updates the pointer in *purray_mem_ptr
 *	   automatically on success.
 */
void *purray_prepend(void **purray_mem_ptr, const void *elem);

/**
 * @brief Append an element to the end of the array.
 * @param purray_mem_ptr Pointer to the array pointer.
 * @param elem Pointer to the element to append.
 * @return Pointer to the array, or `NULL` on failure.
 *		 On failure, the original array remains valid.
 *
 * @note This function updates the pointer in *purray_mem_ptr
 *	   automatically on success.
 */
void *purray_append(void **purray_mem_ptr, const void *elem);

// ============================================================================
// Search functions
// ============================================================================

/**
 * @brief Find an element in the array (forward search).
 * @param purray_mem Pointer to the array memory.
 * @param elem Pointer to the element to find.
 * @param start Starting index for the search.
 * @return Index of the first matching element, or `SIZE_MAX` if not found.
 */
size_t purray_find(const void *purray_mem, const void *elem, size_t start);

/**
 * @brief Find an element in the array (reverse search).
 * @param purray_mem Pointer to the array memory.
 * @param elem Pointer to the element to find.
 * @param start Starting index for the search (search goes backwards).
 * @return Index of the first matching element (from the end),
 *		 or `SIZE_MAX` if not found.
 */
size_t purray_rfind(const void *purray_mem, const void *elem, size_t start);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // PURRAY_H
