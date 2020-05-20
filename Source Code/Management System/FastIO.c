#define  _CRT_SECURE_NO_WARNINGS 
#include "FastIO.h"
#include "DatabaseHeader.h"

void SaveFile(Node* head) {

	remove("data.dat");
	FILE* fp = fopen("data.dat", "wb");

	int count = 0;
	Node* temp = head;
	while (1) {
		count++;
		if (temp->next != NULL) {
			temp = temp->next;
		}
		else {
			break;
		}
	}

	fwrite(&count, sizeof(count), 1, fp);
	fwrite(&StudentCount, sizeof(StudentCount), 1, fp);

	temp = head;

	while (1) {
		fwrite(&temp->data, sizeof(Record), 1, fp);
		fflush(fp);
		if (temp->next != NULL) {
			temp = temp->next;
		}
		else {
			break;
		}
	}

	for (int i = 1; i <= StudentCount; i++) {
		fwrite(&Students[i], sizeof(Student), 1, fp);
	}

	fflush(fp);

	fclose(fp);
}

void ReadFromFile() {
	FILE* fp = fopen("data.dat", "rb");

	if (fp == NULL) {
		printf("数据文件读取失败！ 正在尝试重新初始化...\n");
		int InitCount = 1;
		int InitStuCount = 1;
		Record InitItem;
		Student InitStudent;
		memset(&InitItem, 0, sizeof(InitItem));
		memset(&InitStudent,0, sizeof(InitStudent));
		FILE* fp = fopen("data.dat", "wb");
		fwrite(&InitCount, sizeof(InitCount), 1, fp);
		fwrite(&InitStuCount, sizeof(InitStuCount), 1, fp);
		fwrite(&InitItem, sizeof(InitItem), 1, fp);
		fwrite(&InitStudent, sizeof(InitStudent), 1, fp);
		fclose(fp);
		ReadFromFile(head);
		return;
	}

	Node* p1, *p2;
	int count;
	fread(&count, sizeof(count), 1, fp);
	fread(&StudentCount, sizeof(StudentCount), 1, fp);

	p1 = (Node*)malloc(sizeof(Node));
	p1->next = NULL;
	fread(&p1->data, sizeof(Record), 1, fp);
	p2 = p1; head = p1;
	count--;

	while(count--){
		p1 = (Node*)malloc(sizeof(Node));
		fread(&p1->data, sizeof(Record), 1, fp);
		fflush(fp); 
		p1->next = NULL;
		p2->next = p1; p2 = p1;
	}

	for (int i = 1; i <= StudentCount; i++) {
		fread(&Students[i], sizeof(Student), 1, fp);
		fflush(fp);
	}

	fclose(fp);
}