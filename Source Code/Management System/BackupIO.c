#define  _CRT_SECURE_NO_WARNINGS 
#include <stdio.h>
#include <stdlib.h>
#include "Structure.h"
#include "BackupIO.h"
#include "DatabaseHeader.h"

void SaveBackupFile(Node* head){
	FILE* fp = fopen("DataBackup.txt", "w");

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
	fprintf(fp, "%d \n", count);

	temp = head;

	while (1) {
		fprintf(fp, "%s ", temp->data.Class);
		fprintf(fp, "%s ", temp->data.StuName);
		fprintf(fp, "%s ", temp->data.Subject);
		fprintf(fp, "%d ", temp->data.StuID);
		fprintf(fp, "%d \n", temp->data.Grade);
		if (temp->next != NULL) {
			temp = temp->next;
		}
		else {
			break;
		}
	}

	fprintf(fp, "%d \n", StudentCount);
	for (int i = 1; i <= StudentCount; i++) {
		fprintf(fp, "%d ", Students[i].id);
		fprintf(fp, "%s ", Students[i].name);
		fprintf(fp, "%d ", Students[i].sex);
		fprintf(fp, "%s ", Students[i].nationality);
		fprintf(fp, "%d ", Students[i].age);
		fprintf(fp, "%s \n", Students[i].address);
	}

	fclose(fp);
}