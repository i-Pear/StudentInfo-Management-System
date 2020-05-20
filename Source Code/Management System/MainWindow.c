#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include "acllib.h"
#include "Structure.h"
#include "DatabaseHeader.h"
#include "QueueControl.h"
#include "FastIO.h"
#include "BackupIO.h"

#define WinWidth 800
#define WinHeight 450

// Here "ViewMode" controls the working mode of MouseEventListener & KeyEventListener
// Edit this value to specify how events will be explained
int ViewMode;

char PasswordReal[7]="000000";
char Password[7];
int PasswordCount;
int EditingPasswordMode = 1;

char KeyWords[100][100];
int KeyWordsCount;

void AnalyzeKeywords(char* str) {
	KeyWordsCount = 0;
	memset(KeyWords, 0, sizeof(KeyWords));
	while (*str == ' ') {
		str++;
	}
	char* p = (char*)str;
	char* q = (char*)str;

	while (*p != '\0') {
		while (*q != ' '&&*q!='\0') {
			q++;
		}
		for (char *sour = p, *dest = KeyWords[++KeyWordsCount]; sour != q; sour++, dest++) {
			*dest = *sour;
		}
		while (*q == ' ') {
			q++;
		}
		p = q;
	}
}

inline int GetCenterX(int charSize,int charCount) {
	return (WinWidth - charSize * charCount) / 2;
}

void PaintMenu() {
	beginPaint();
	clearDevice();
	setTextSize(45);
	paintText(310, 50, "主菜单");
	paintText(60, 150, "1：查询成绩");
	paintText(460, 150, "2：查询学生");
	paintText(60, 225, "3：录入/修改成绩");
	paintText(460, 225, "4：初始化班级");
	paintText(60, 300, "5：新增学生信息");
	paintText(460, 300, "6：导出数据");
	paintText(60, 375, "7：口令维护");
	paintText(460, 375, "8：统计功能");
	endPaint();
}

void PaintWelcome() {
	beginPaint();
	setTextFont("微软雅黑 Light");
	clearDevice();
	setTextSize(60);
	paintText(GetCenterX(45,13), 150, "欢迎来到学生信息管理系统！");
	endPaint();
}

void PaintLogin() {
	beginPaint();
	clearDevice();
	setTextSize(60);
	paintText(GetCenterX(45,12),70, "欢迎使用 请输入用户密码");
	// Draw two horizontal lines
	line(250, 220, 550, 220);
	line(250, 270, 550, 270);
	// Draw the vertical lines
	for (int i = 0; i <= 6; i++) {
		line(250 + 50 * i,220, 250 + 50 * i, 270);
	}
	// Draw Passwords
	for (int i = 0; i < PasswordCount; i++) {
		paintText(262 + 50 * i, 220, "*");
	}
	endPaint();
}

void LoginPageListener(char chr) {
	if (PasswordCount < 6) {
		Password[PasswordCount++] = chr;
		PaintLogin();
	}
	else {
		Password[6] = '\0';
		if (strcmp(Password, PasswordReal) == 0) {
			ViewMode = 2;
			PasswordCount = 0;
			PaintMenu();
		}
		else {
			PasswordCount = 0;
			msgBox("Sorry", "密码错误，请重新输入", 0);
			PaintLogin();
		}
	}
	return;
}

void PaintEditPassword() {
	beginPaint();
	clearDevice();
	setTextSize(45);
	if (EditingPasswordMode) {
		paintText(200, 75, "请输入新密码");
	}
	else {
		paintText(180, 75, "请再次输入新密码");
	}
	// Draw two horizontal lines
	line(250, 220, 550, 220);
	line(250, 270, 550, 270);
	// Draw the vertical lines
	for (int i = 0; i <= 6; i++) {
		line(250 + 50 * i, 220, 250 + 50 * i, 270);
	}
	// Draw Passwords
	for (int i = 0; i < PasswordCount; i++) {
		paintText(262 + 50 * i, 225, "*");
	}
	endPaint();
}

void EditPasswordListener(char chr) {
	if (PasswordCount < 6) {
		Password[PasswordCount++] = chr;
		PaintEditPassword();
	}
	else {
		Password[6] = '\0';
		if (EditingPasswordMode) {
			EditingPasswordMode = !EditingPasswordMode;
			Password[6] = '\0';
			strcpy(PasswordReal, Password);
			PasswordCount = 0;
			PaintEditPassword();
		}
		else {
			EditingPasswordMode = !EditingPasswordMode;
			Password[6] = '\0';
			if (strcmp(Password, PasswordReal) == 0) {
				//Write password to file
				FILE* fp;
				if ((fp = fopen("password", "w")) != NULL) {
					fprintf(fp, "%s", Password);
					fclose(fp);
				}
				else {
					msgBox("密码修改异常", "无法访问密码文件，请检查权限", 0);
				}
				ViewMode = 2;
				PasswordCount = 0;
				PaintMenu();
			}
			else {
				msgBox("密码输入错误", "两次输入密码不一致，请重新输入！", 0);
				PasswordCount = 0;
				PaintEditPassword();
			}
		}
	}
	return;
}

void SearchRecord() {
	ViewMode = 0;
	PaintWelcome();
	printf("(支持多关键词模糊搜索功能 将筛选出匹配所有关键词的记录)\n");
	printf("(支持键值匹配 可用关键字 stu class grade subject 用法如stu=10)\n");
	printf("请输入搜索关键词 以空格分隔：\n");

	Node* temp = head;
	while (1) {
		sprintf(temp->data.keyString, "stu=%d stu=%s class=%s grade=%d subject=%s", temp->data.StuID, temp->data.StuName, temp->data.Class, temp->data.Grade, temp->data.Subject);
		if (temp->next != NULL) {
			temp = temp->next;
		}
		else {
			break;
		}
	}

	char KeyWordsLine[2000];
	memset(&KeyWordsLine, 0, sizeof(KeyWordsLine));
	gets_s(KeyWordsLine, 2000);
	AnalyzeKeywords(KeyWordsLine);
	QueueInitialize();

	temp = head;
	while (1) {
		int ifok = 1;
		for (int i = 1; i <= KeyWordsCount; i++) {
			if (strstr(temp->data.keyString, KeyWords[i]) == NULL) {
				ifok = 0;
				break;
			}
		}
		if (ifok&&temp->data.Typed!=0) {
			QueueInsert(&temp->data);
		}

		if (temp->next != NULL) {
			temp = temp->next;
		}
		else {
			break;
		}
	}

	for (int i = 1; i <= QueueCount; i++) {
		Record* temp = list[i];
		printf("%2d  %5s  %5s  %10s  %3d\n", temp->StuID, temp->StuName, temp->Class, temp->Subject, temp->Grade);
	}

	printf("-------------------\n");
	ViewMode = 2;
	PaintMenu();
}

void SearchStudent() {
	ViewMode = 0;
	PaintWelcome();
	printf("(支持多关键词模糊搜索功能 将筛选出匹配所有关键词的学生)\n");
	printf("(支持键值匹配 可用关键字 id name sex age address 用法如id=10)\n");
	printf("请输入搜索关键词 以空格分隔：\n");

	for (int i = 1; i <= StudentCount; i++) {
		sprintf(Students[i].keyString, "id=%d name=%s nation=%s sex=%d age=%d address=%s", Students[i].id, Students[i].name, Students[i].nationality, Students[i].sex, Students[i].age, Students[i].address);
	}

	char KeyWordsLine[2000];
	memset(&KeyWordsLine, 0, sizeof(KeyWordsLine));
	gets_s(KeyWordsLine, 2000);
	AnalyzeKeywords(KeyWordsLine);
	QueueInitialize();

	for(int j=1;j<=StudentCount;j++){
		int ifok = 1;
		for (int i = 1; i <= KeyWordsCount; i++) {
			if (strstr(Students[j].keyString, KeyWords[i]) == NULL) {
				ifok = 0;
				break;
			}
		}
		if (ifok) {
			QueueInsert(&Students[j]);
		}
	}

	for (int i = 1; i <= QueueCount; i++) {
		Student* temp = list[i];
		printf("id=%2d name=%5s nation=%5s sex=%d age=%2d address=%s \n", temp->id, temp->name, temp->nationality, temp->sex, temp->age, temp->address);
	}

	printf("-------------------\n");
	ViewMode = 2;
	PaintMenu();
}

void EditData() {
	ViewMode = 0;
	PaintWelcome();
	printf("(请输入修改成绩的范围)\n");
	printf("(支持键值匹配 可用关键字 stu class grade subject 用法如stu=10)\n");
	printf("请输入范围关键词 以空格分隔：\n");

	Node* temp = head;
	while (1) {
		sprintf(temp->data.keyString, "stu=%d stu=%s class=%s grade=%d subject=%s typed=%d", temp->data.StuID, temp->data.StuName, temp->data.Class, temp->data.Grade, temp->data.Subject,temp->data.Typed);
		if (temp->next != NULL) {
			temp = temp->next;
		}
		else {
			break;
		}
	}

	char KeyWordsLine[2000];
	memset(&KeyWordsLine, 0, sizeof(KeyWordsLine));
	gets_s(KeyWordsLine, 2000);
	AnalyzeKeywords(KeyWordsLine);
	QueueInitialize();

	temp = head;
	while (1) {
		int ifok = 1;
		for (int i = 1; i <= KeyWordsCount; i++) {
			if (strstr(temp->data.keyString, KeyWords[i]) == NULL) {
				ifok = 0;
				break;
			}
		}
		if (ifok) {
			QueueInsert(&temp->data);
		}

		if (temp->next != NULL) {
			temp = temp->next;
		}
		else {
			break;
		}
	}

	printf("-------------------\n");

	for (int i = 1; i <= QueueCount; i++) {
		Record* temp = list[i];
		printf("%d  %s  %s  %s :",temp->StuID, temp->StuName, temp->Class, temp->Subject);
		scanf("%d", &temp->Grade);
		temp->Typed = 1;
		printf("\n");
	}
	SaveFile(head);
	printf("-------------------\n");
	ViewMode = 2;
	PaintMenu();
}

void OutputData() {
	printf("----------------\n正在导出数据…\n");
	SaveBackupFile(head);
	printf("数据导出完成！\n----------------\n");
}

void AddStudentInfo() {
	int n;
	printf("请输入要输入学生的数量：\n");
	scanf("%d", &n);
	for (int i = 1; i <= n; i++) {
		Student temp;
		temp.id = StudentCount + 1;
		printf("请依次输入第%d个学生的姓名 年龄 民族 性别 地址：\n",i);
		scanf("%s%d%s%d%s", &temp.name, &temp.age, &temp.nationality, &temp.sex, &temp.address);
		Students[++StudentCount] = temp;
	}
	SaveFile(head);
	printf("新增完成！\n------------------\n");
}

void AddClass() {
	char className[100];
	printf("请输入班级名称：\n");
	scanf("%s", className);
	int stuCount;
	int subjectCount;
	printf("请输入新班级内学生的数量：\n");
	scanf("%d", &stuCount);
	printf("请输入新班级所学科目数量：\n");
	scanf("%d", &subjectCount);
	char stuName[100][100];
	char subjectName[100][100];
	printf("请依次输入学生姓名，每行一个：\n");
	for (int i = 1; i <= stuCount; i++) {
		scanf("%s", stuName[i]);
	}
	printf("请依次输入学科名称，每行一个：\n");
	for (int i = 1; i <= subjectCount; i++) {
		scanf("%s", &subjectName[i]);
	}

	Node* tail = head;
	while (tail->next != NULL) {
		tail = tail->next;
	}

	for (int i = 1; i <= stuCount; i++) {
		int id = 0;

		for (int j = 1; j <= StudentCount; j++) {
			if (strcmp(stuName[i],Students[j].name)==0) {
				id = Students[j].id;
				break;
			}
		}

		for (int j = 1; j <= subjectCount; j++) {
			Node* newone = malloc(sizeof(Node));
			newone->next = NULL;
			newone->data.Typed = 0;
			newone->data.Grade = 0;
			strcpy(newone->data.Subject, subjectName[j]);
			strcpy(newone->data.StuName, stuName[i]);
			strcpy(newone->data.Class, className);
			newone->data.StuID = tail->data.StuID + 1;
			newone->data.Subject;
			tail->next = newone;
			tail = newone;
		}
	}
	SaveFile(head);
	printf("班级创建完成\n");
}

void MouseEventListener(int x, int y, int button, int event) {
	if (event == BUTTON_DOWN) {
		if (ViewMode == -1) {
			//Welcome page
			ViewMode = 1;
			PaintLogin();
		}
	}
}

void CharEventListener(char chr) {
	if (ViewMode == 1) {
		// Login Page
		LoginPageListener(chr);
		return;
	}
	if (ViewMode == 5) {
		// Edit Password
		EditPasswordListener(chr);
		return;
	}
	if (ViewMode == 2) {
		// Main menu
		if (chr == '1') {
			// 1：查询成绩
			SearchRecord();
		}
		if (chr == '2') {
			// 2：查询学生
			SearchStudent();
		}
		if (chr == '3') {
			// 3：录入/修改成绩
			EditData();
		}
		if (chr == '4') {
			// 4：初始化班级
			AddClass();
		}
		if (chr == '5') {
			// 5：新增学生信息
			AddStudentInfo();
		}
		if (chr == '6') {
			// 6：导出数据
			OutputData();
		}
		if (chr == '7') {
			// 7：口令维护
			ViewMode = 5;
			PaintEditPassword();
		}
		if (chr == '8') {
			// 8：统计功能
			ViewMode = 5;
			printf("-----------------\n");
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
			printf("系统中目前共有%d项数据\n",count);
			printf("学生记录共存在%d项\n", StudentCount);
			printf("------------------\n");
		}
		return;
	}
}

int Setup() {
	//initConsole();
	initWindow("Management System GUI",5,5,WinWidth,WinHeight);
	FILE* fp;
	if ((fp = fopen("password", "r")) != NULL) {
		fscanf(fp, "%s", PasswordReal);
		fclose(fp);
	}

	ReadFromFile();

	PaintWelcome();
	ViewMode = -1;
	registerMouseEvent(&MouseEventListener);
	registerCharEvent(&CharEventListener);
	return 0;
}