#include <stdio.h>

#include "purray.h"

int main(void) {
	int *arr = (int*)purray_alloc(10, sizeof(int));
	if (!arr) return 1;

	const int tmp[] = { 1, 2, -3, -4, 5, 6, 77, -888, 9999, 1000000};
	for (unsigned i = 0; i < 10; i++) purray_prepend(&arr, &tmp[i]);

	for (size_t i = 0; i < 10; i++) printf("%d ", arr[i]);
	return 0;
}
