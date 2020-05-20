#pragma once
#include "Structure.h"
#include <string.h>

static void swap(Record* a, Record* b);

static int cmp(Record* a, Record* b);

void quicksort(Record* arr, int left, int right);