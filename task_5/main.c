#include <limits.h>
#include <stdio.h>
#include <string.h>

void* linear_search(void* array, const size_t count, const size_t element_size, const void* target, size_t* output_index) {
	unsigned char* ptr = array;

	for (size_t i = 0; i < count; i++) {
		if (memcmp(ptr + i * element_size, target, element_size) == 0) {
			if (output_index) {
				*output_index = i;
			}

			return ptr + i * element_size;
		}
	}

	if (output_index) {
		*output_index = SIZE_MAX;
	}

	return NULL;
}

int main(void) {
	int numbers[] = {1, 2, 3, 5, 6, 7, 8, 9, 0};
	double numbers_d[] = {1.1, 2.2, 3.3, 4.4};

	size_t pos1, pos2;

	const int* fi = linear_search(numbers, sizeof(numbers) / sizeof(int), sizeof(int), &(int){0}, &pos1);
	const double* fd = linear_search(numbers_d, sizeof(numbers_d) / sizeof(double), sizeof(double), &(double){4.4}, &pos2);

	if (fi) {
		printf("int %d at %llu\n", *fi, pos1);
	}

	if (fd) {
		printf("double %.1f at %llu\n", *fd, pos2);
	}
}
