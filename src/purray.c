#include "purray.h"

static inline size_t align_value_up2(size_t value, size_t alignment) {
	return (value + alignment - 1) & ~(alignment - 1);
}

static inline size_t align_value_down2(size_t value, size_t alignment) {
	return value & ~(alignment - 1);
}

size_t purray_len(void *purray_mem) {
	if (!purray_mem) return SIZE_MAX;

	const PurrayHeader *header = PURRAY_MEM_TO_HEADER(purray_mem);
	if (header->magic != PURRAY_HEADER_MAGIC) return SIZE_MAX;

	return header->num_elems;
}

size_t purray_elem_size(void *purray_mem) {
	if (!purray_mem) return SIZE_MAX;

	const PurrayHeader *header = PURRAY_MEM_TO_HEADER(purray_mem);
	if (header->magic != PURRAY_HEADER_MAGIC) return SIZE_MAX;

	return header->elem_size;
}

void *purray_alloc(size_t capacity, size_t elem_size) {
	capacity = align_value_up2(capacity, PURRAY_CAPACITY_GRANULARITY);
	if (capacity > SIZE_MAX / elem_size) return NULL;
	
	const size_t data_size = capacity * elem_size;
	const size_t full_size = sizeof(PurrayHeader) + data_size;

	void *raw_mem = malloc(full_size);
	if (!raw_mem) return NULL;

	PurrayHeader *header = (PurrayHeader*)raw_mem;
	header->magic = PURRAY_HEADER_MAGIC;
	header->mem = PURRAY_HEADER_TO_MEM(header);
	header->elem_size = elem_size;
	header->num_elems = 0;
	header->capacity = capacity;

	return header->mem;
}

void *purray_realloc(void *purray_mem, size_t new_capacity) {
	if (!purray_mem) return NULL;

	PurrayHeader *old_header = PURRAY_MEM_TO_HEADER(purray_mem);
	if (old_header->magic != PURRAY_HEADER_MAGIC) return NULL;

	new_capacity = align_value_up2(new_capacity, PURRAY_CAPACITY_GRANULARITY);
	if (new_capacity > SIZE_MAX / old_header->elem_size) return NULL;

	const size_t data_size = old_header->elem_size * new_capacity;
	const size_t full_size = sizeof(PurrayHeader) + data_size;

	void *tmp = realloc(old_header, full_size);
	if (!tmp) return NULL;

	PurrayHeader *new_header = (PurrayHeader*)tmp;
	new_header->mem = PURRAY_HEADER_TO_MEM(new_header);
	new_header->capacity = new_capacity;

	if (new_header->num_elems > new_capacity) new_header->num_elems = new_capacity;

	return new_header->mem;
}

void *purray_dup(const void *purray_mem) {
	if (!purray_mem) return NULL;

	const PurrayHeader *old_header = PURRAY_MEM_TO_HEADER(purray_mem);
	if (old_header->magic != PURRAY_HEADER_MAGIC) return NULL;

	if (old_header->capacity > SIZE_MAX / old_header->elem_size) return NULL;
	const size_t data_size = old_header->elem_size * old_header->capacity;
	const size_t full_size = sizeof(PurrayHeader) + data_size;

	void *raw_mem = malloc(full_size);
	if (!raw_mem) return NULL;

	PurrayHeader *new_header = (PurrayHeader*)raw_mem;
	memcpy(new_header, old_header, full_size);

	new_header->mem = PURRAY_HEADER_TO_MEM(new_header);
	return new_header->mem;
}

void purray_free(void *purray_mem) {
	if (!purray_mem) return;

	PurrayHeader *header = PURRAY_MEM_TO_HEADER(purray_mem);
	if (header->magic != PURRAY_HEADER_MAGIC) return;

	free(header);
}

void *purray_place(void *purray_mem, const void *elem, size_t pos) {
	if (!purray_mem) return NULL;

	PurrayHeader *header = PURRAY_MEM_TO_HEADER(purray_mem);
	if (header->magic != PURRAY_HEADER_MAGIC || pos > header->num_elems) return NULL;

	if (header->num_elems >= header->capacity) {
		const size_t new_capacity = header->num_elems + PURRAY_CAPACITY_GRANULARITY;
		purray_mem = purray_realloc(purray_mem, new_capacity);
		if (!purray_mem) return NULL;

		header = PURRAY_MEM_TO_HEADER(purray_mem);
	}

	const size_t shifted_size = header->elem_size * (header->num_elems - pos);
	char *insert_at_ptr = (char*)purray_mem + header->elem_size * pos;
	char *shifted_ptr = insert_at_ptr + header->elem_size;
	memmove(shifted_ptr, insert_at_ptr, shifted_size);

	memcpy(insert_at_ptr, elem, header->elem_size);
	header->num_elems += 1;

	return purray_mem;
}

void *purray_remove(void *purray_mem, size_t pos, size_t count) {
	if (!purray_mem) return NULL;

	PurrayHeader *header = PURRAY_MEM_TO_HEADER(purray_mem);
	if (header->magic != PURRAY_HEADER_MAGIC || pos >= header->num_elems) return NULL;

	const size_t old_capacity = header->capacity;
	if (pos + count > header->num_elems) count = header->num_elems - pos;

	const size_t shifted_size = header->elem_size * (header->num_elems - (pos + count));
	char *shifted_dst_ptr = (char*)purray_mem + header->elem_size * pos;
	char *shifted_src_ptr = shifted_dst_ptr + header->elem_size * count;
	memmove(shifted_dst_ptr, shifted_src_ptr, shifted_size);
	header->num_elems -= count;

	const size_t new_capacity = align_value_up2(header->num_elems, PURRAY_CAPACITY_GRANULARITY);
	if (new_capacity != old_capacity && new_capacity > 0) {
		purray_mem = purray_realloc(purray_mem, new_capacity);
		if (!purray_mem) return NULL;

		header = PURRAY_MEM_TO_HEADER(purray_mem);
	}

	return purray_mem;
}

void *purray_resize(void **purray_mem_ptr, size_t new_capacity) {
	if (!purray_mem_ptr || !*purray_mem_ptr) return NULL;

	void *tmp = purray_realloc(*purray_mem_ptr, new_capacity);
	if (!tmp) return NULL;

	*purray_mem_ptr = tmp;
	return tmp;
}

void *purray_insert(void **purray_mem_ptr, const void *elem, size_t pos) {
	if (!purray_mem_ptr || !*purray_mem_ptr) return NULL;

	void *tmp = purray_place(*purray_mem_ptr, elem, pos);
	if (!tmp) return NULL;

	*purray_mem_ptr = tmp;
	return tmp;
}

void *purray_prepend(void **purray_mem_ptr, const void *elem) {
	return purray_insert(purray_mem_ptr, elem, 0);
}

void *purray_append(void **purray_mem_ptr, const void *elem) {
	if (!purray_mem_ptr || !*purray_mem_ptr) return NULL;

	PurrayHeader *header = PURRAY_MEM_TO_HEADER(*purray_mem_ptr);
	if (header->magic != PURRAY_HEADER_MAGIC) return NULL;

	return purray_insert(purray_mem_ptr, elem, header->num_elems);
}

size_t purray_find(const void *purray_mem, const void *elem, size_t start) {
	if (!purray_mem || !elem) return SIZE_MAX;

	const PurrayHeader *header = PURRAY_MEM_TO_HEADER(purray_mem);
	if (header->magic != PURRAY_HEADER_MAGIC) return SIZE_MAX;

	char *array_elem = (char*)header->mem + header->elem_size * start;
	for (size_t i = start; i < header->num_elems; i++) {
		if (!memcmp(array_elem, elem, header->elem_size)) return i;

		array_elem += header->elem_size;
	}

	return SIZE_MAX;
}

size_t purray_rfind(const void *purray_mem, const void *elem, size_t start) {
	if (!purray_mem || !elem) return SIZE_MAX;

	const PurrayHeader *header = PURRAY_MEM_TO_HEADER(purray_mem);
	if (header->magic != PURRAY_HEADER_MAGIC || !header->num_elems) return SIZE_MAX;

	if (start == SIZE_MAX) start = header->num_elems - 1;
	if (start >= header->num_elems) return SIZE_MAX;

	char *array_elem = (char*)header->mem + header->elem_size * start;
	for (size_t i = start + 1; i > 0; --i) {
		if (!memcmp(array_elem, elem, header->elem_size)) return i;

		array_elem -= header->elem_size;
	}
	return SIZE_MAX;
}
