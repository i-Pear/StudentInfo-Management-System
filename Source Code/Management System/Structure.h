#pragma once
typedef struct {
	int id;
	char name[10];
	int sex;
	char nationality[10];
	int age;
	char address[100];
	char keyString[200];
} Student;

typedef struct {
	int Typed;
	int StuID;
	char StuName[30];
	char Class[30];
	int Grade;
	char Subject[30];
	char keyString[200];
} Record;

typedef struct ListNode {
	Record data;
	struct ListNode * next;
}Node;
