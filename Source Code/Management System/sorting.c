#include "Structure.h"
#include "Sorting.h"

static void swap(Record* a, Record* b) {
	Record temp = *a;
	*a = *b;
	*b = temp;
}

static int cmp(Record* a, Record* b) {
	if (strcmp(a->Class,b->Class)) {
		return strcmp(a->Class, b->Class);
	}
	return a->StuID > b->StuID;
}

void bubblesort(Record* arr, int left, int right) {
	for (int i = left; i < right; i++) {
		for (int j = left; j <= right - i; j++) {
			if (cmp(&arr[j], &arr[j + 1])) {
				swap(&arr[j], &arr[j + 1]);
			}
		}
	}
}

void quicksort(Record* arr,int left, int right) {
	if (left >= right) {
		return;
	}
	int i=left, j=right;
	while (i < j) { 
		while (cmp(&arr[j] ,&arr[left]) && i < j)
			j--;
		while (!cmp(&arr[i], &arr[left]) && i < j)
			i++;
		swap(&arr[i], &arr[j]);
	}

	swap(&arr[left], &arr[i]);

	quicksort(arr,left, i - 1);
	quicksort(arr,i + 1, right);
}