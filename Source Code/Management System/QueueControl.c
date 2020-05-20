#pragma once
#include "Structure.h"
#include "Sorting.h"
#include "QueueControl.h"

void QueueInitialize() {
	QueueCount = 0;
}

void QueueInsert(void* rec) {
	list[++QueueCount] = rec;
}

void QueueSort() {
	quicksort(&list, 1, QueueCount);
}