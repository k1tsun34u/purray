#include <stdio.h>

#include "purray.h"

void print_array(const int *arr, size_t len);

int main(void) {
	int *arr = (int*)purray_alloc(10, sizeof(int));
	if (!arr) return 1;

	const int tmp[] = { 1, 2, -3, -4, 5, 6, 77, -888, 9999, 1000000};
	for (unsigned i = 0; i < 10; i++) purray_prepend(&arr, &tmp[i]);
	print_array(arr, purray_len(arr));

	purray_free(arr);

	arr = (int*)purray_alloc(0, sizeof(int));
	for (unsigned i = 0; i < 10; i++) purray_append(&arr, &tmp[i]);
	
	int elem = 3456789;
	purray_insert(&arr, &elem, 2);
	print_array(arr, purray_len(arr));

	arr = purray_remove(arr, 3, 5);
	print_array(arr, purray_len(arr));

	printf("Index of elem(%d) = %zu\n", elem, purray_find(arr, &elem, 0));

	elem = 512;
	printf("Index of elem(%d) = %zu\n", elem, purray_find(arr, &elem, 0));
	printf("SIZE_MAX = %zu\n", SIZE_MAX);
	
	purray_free(arr);
	return 0;
}

void print_array(const int *arr, size_t len) {
	for (size_t i = 0; i < len; i++) printf("%d ", arr[i]);
	printf("\n");
}
