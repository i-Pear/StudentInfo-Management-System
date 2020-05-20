#pragma once
#include <stdio.h>
#include "Structure.h"
#include <string.h>

static void quick_sort_list_details(Node* start, Node* end, int(*compare)(Record, Record));

static void swap(Record* a, Record* b) {
	Record temp = *a;
	*a = *b;
	*b = temp;
}

static int cmp(Record* a, Record* b) {
	if (strcmp(a->Class , b->Class)) {
		return strcmp(a->Class, b->Class);
	}
	return a->StuID > b->StuID;
}

void quick_sort_list(Node* start,int(*compare)(Record,Record)) {
	quick_sort_list_details(start, NULL,compare);
}

static void quick_sort_list_details(Node* start, Node* end, int(*compare)(Record, Record)){
	if (start == NULL || start == end) return;

	Node* p1 = start;
	Node* p2 = start->next;

	while (p2 != end) {
		if (compare(p1->data,p2->data)) {
			p1 = p1->next;
			swap(&p1->data, &p2->data);
		}
		p2 = p2->next;
	}
	swap(&p1->data, &start->data);

	quick_sort_list_details(start, p1,compare);
	quick_sort_list_details(p1->next, end,compare);
}