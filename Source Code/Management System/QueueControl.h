#pragma once
#include "Structure.h"
#include "Sorting.h"

int QueueCount;
void* list[1000];

void QueueInitialize();

void QueueInsert(void* rec);

void QueueSort();
