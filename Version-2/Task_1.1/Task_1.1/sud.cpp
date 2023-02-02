#include "datast.h"
#include "func.h"
void Sudsolve() {
	system("cls");
	char FileName[20];
	printf("请输入数独文件:\n");
	scanf("%s", FileName);
	int len = strlen(FileName);
	while (strcmp(FileName + len - 3, "txt")) {
		printf("格式错误，请重新输入\n");
		scanf("%s", FileName);
		len = strlen(FileName);
	}
	SudToSat(FileName);

	FileName[len - 3] = 'c', FileName[len - 2] = 'n', FileName[len - 1] = 'f';
	problem* P = (problem*)malloc(sizeof(problem));
	FILE* fp = fopen(FileName, "r");
	read(fp, P);
	fclose(fp);

	time_t t1, t2;
	t1 = clock();
	int state = solve(P);
	t2 = clock();
	if (state == 1) printf("求解成功！\n");
	if (state == 0) printf("数独无解\n");
	if (state == -1) printf("求解超时\n");

	len = strlen(FileName);
	FileName[len - 3] = 'r', FileName[len - 2] = 'e', FileName[len - 1] = 's';
	fp = fopen(FileName, "w+");
	Ansprint(fp, P, state, t2 - t1);
	fclose(fp);

	fp = fopen(FileName, "r");
	ShowSud(fp);
	printf("\n用时: %d ms", t2 - t1);
	pfree(P);
}
int VarTrans(int i, int j, int n) {
	return (i - 1) * 81 + (j - 1) * 9 + n;
}
void BasicPrint(FILE* fp) {
	for (int i = 1; i <= 9; i++) {//每空至少一个数字
		for (int j = 1; j <= 9; j++) {
			for (int n = 1; n <= 9; n++) {
				fprintf(fp, "%d ", VarTrans(i, j, n));
			}
			fprintf(fp, "0\n");
		}
	}
	for (int i = 1; i <= 9; i++) {//每行每个数字都出现
		for (int n = 1; n <= 9; n++) {
			for (int j = 1; j <= 9; j++) {
				fprintf(fp, "%d ", VarTrans(i, j, n));
			}
			fprintf(fp, "0\n");
		}
	}
	for (int j = 1; j <= 9; j++) {//每列每个数字都出现
		for (int n = 1; n <= 9; n++) {
			for (int i = 1; i <= 9; i++) {
				fprintf(fp, "%d ", VarTrans(i, j, n));
			}
			fprintf(fp, "0\n");
		}
	}
	for (int p = 0; p < 3; p++) {//每区域每个数字都出现
		for (int q = 0; q < 3; q++) {
			for (int i = 1; i <= 3; i++) {
				for (int n = 1; n <= 9; n++) {
					for (int j = 1; j <= 3; j++) {
						fprintf(fp, "%d ", VarTrans(3 * p + i, 3 * q + j, n));
					}
				}
			}
			fprintf(fp, "0\n");
		}
	}
	for (int i = 1; i <= 9; i++) {//每行每个数字不重复出现
		for (int n = 1; n <= 9; n++) {
			for (int j1 = 1; j1 < 9; j1++) {
				for (int j2 = j1 + 1; j2 <= 9; j2++) {
					fprintf(fp, "%d %d 0\n", (-1) * VarTrans(i, j1, n), (-1) * VarTrans(i, j2, n));
				}
			}
		}
	}
	for (int j = 1; j <= 9; j++) {//每列每个数字不重复出现
		for (int n = 1; n <= 9; n++) {
			for (int i1 = 1; i1 < 9; i1++) {
				for (int i2 = i1 + 1; i2 <= 9; i2++) {
					fprintf(fp, "%d %d 0\n", (-1) * VarTrans(i1, j, n), (-1) * VarTrans(i2, j, n));
				}
			}
		}
	}
	for (int p = 0; p < 3; p++) {//每区域每个数字不重复出现
		for (int q = 0; q < 3; q++) {
			for (int n = 1; n <= 9; n++) {
				for (int k1 = 0; k1 <= 7; k1++) {
					for (int k2 = k1 + 1; k2 <= 8; k2++) {
						fprintf(fp, "%d %d 0\n",
							(-1) * VarTrans(3 * p + k1 / 3 + 1, 3 * q + k1 % 3 + 1, n),
							(-1) * VarTrans(3 * p + k2 / 3 + 1, 3 * q + k2 % 3 + 1, n));
					}
				}
			}
		}
	}
}
void ShowSud(FILE* fp) {
	int n;
	char c;
	fscanf(fp, "%c%d", &c, &n);
	if (n != 1) return;
	for (int k = 1; k <= 81; k++) {
		if (!((k - 1) % 9)) printf("\n");
		fscanf(fp, " %c", &c);
		for (int i = 1; i <= 9; i++) {
			fscanf(fp, "%d", &n);
			if (n > 0) printf("%3d", i);
		}
	}
}
void SudToSat(char FileName[]) {
	FILE* fp1, * fp2;
	fp1 = fopen(FileName, "r");
	char Output[20];
	strcpy(Output, FileName);
	int len = strlen(FileName);
	Output[len - 3] = 'c', Output[len - 2] = 'n', Output[len - 1] = 'f';
	fp2 = fopen(Output, "w");
	sud* k;
	k = (sud*)malloc(sizeof(sud));
	Sudread(fp1, k);
	fprintf(fp2, "p cnf %d %d\n", 729, 9000 + k->num);//共729个变元，原始规则包含9000条语句	
	for (int i = 1; i <= 9; i++) {
		for (int j = 1; j <= 9; j++) {
			if (k->a[i][j]) fprintf(fp2, "%d 0\n", (i - 1) * 81 + (j - 1) * 9 + k->a[i][j]);
		}
	}
	BasicPrint(fp2);//打印原始规则
	fclose(fp1), fclose(fp2);
}
void Sudprint(FILE* fp, sud* k) {
	for (int i = 1; i <= 9; i++) {
		for (int j = 1; j <= 9; j++) {
			fprintf(fp, "%d ", k->a[i][j]);
		}
		fprintf(fp, "\n");
	}
}
void Sudread(FILE* fp, sud* k) {
	k->num = 0;
	for (int i = 1; i <= 9; i++) {
		for (int j = 1; j <= 9; j++) {
			fscanf(fp, "%d", &k->ori[i][j]);
			k->a[i][j] = k->ori[i][j];
			if (k->a[i][j]) k->num++;
		}
	}
}
void Candig(sud* k, int x, int y) {
	char temp[10] = "temp.txt";
	FILE* fp1, * fp2;
	int state;
	for (int i = 1; i <= 9; i++) {//试填除原来数字之外的8个数字，若都不能满足说明可以挖洞
		if (i == k->ori[x][y]) continue;
		fp1 = fopen(temp, "w");
		fp2 = fopen("temp.cnf", "r");
		k->a[x][y] = i;
		Sudprint(fp1, k);
		fclose(fp1);
		SudToSat(temp);
		problem* P = (problem*)malloc(sizeof(problem));
		read(fp2, P);
		fclose(fp2);
		state = solve(P);
		if (state == 1) {//若能满足，说明不能挖洞
			k->a[x][y] = k->ori[x][y];
			pfree(P);
			return;
		}
		pfree(P);
	}
	k->a[x][y] = 0;
	return;
}
void DigHole(sud* k) {
	FILE* fp = fopen("temp.cnf", "w");
	fclose(fp);
	for (int i = 1; i <= 9; i++) {
		if (i % 2) {//S型行进
			for (int j = 1; j <= 9; j++) {
				Candig(k, i, j);
			}
		}
		else {
			for (int j = 9; j >= 1; j--) {
				Candig(k, i, j);
			}
		}
	}
}
void ShowAns(sud* k) {
	for (int i = 1; i <= 9; i++) {
		for (int j = 1; j <= 9; j++) {
			printf("%3d", k->ori[i][j]);
		}
		printf("\n");
	}
}
sud* Sudcreate() {
	int x, y, v;
	srand(time(NULL));
	sud* k = (sud*)malloc(sizeof(sud));
	k->num = 3;
	memset(k->a, 0, sizeof(k->a));
	for (int i = 0; i < 3; i++) {
		x = rand() % 3 + 1 + i * 3;//随机确定数独一个位置的数值
		y = rand() % 3 + 1 + i * 3;
		v = rand() % 3 + 1 ;
		k->a[x][y] = v;
	}
	FILE* fp = fopen("sud.txt", "w");
	Sudprint(fp, k);
	fclose(fp);
	free(k);

	char temp[10] = "sud.txt";
	SudToSat(temp);
	problem* P = (problem*)malloc(sizeof(problem));
	fp = fopen("sud.cnf", "r");
	read(fp, P);
	time_t t1, t2;
	t1 = clock();
	solve(P);//求解只含有一个数字的数独
	t2 = clock();
	fclose(fp);

	fp = fopen("sud.res", "w");
	Ansprint(fp, P, 1, t2 - t1);
	fclose(fp);
	pfree(P);

	FILE* fp1 = fopen("sud.res", "r");
	FILE* fp2 = fopen("sud.txt", "w");
	fShowSud(fp1, fp2);//生成未挖洞的数独格局
	fclose(fp1), fclose(fp2);

	fp = fopen("sud.txt", "r");
	k = (sud*)malloc(sizeof(sud));
	Sudread(fp, k);
	fclose(fp);
	DigHole(k);//挖洞
	return k;
}
void fShowSud(FILE* fp1, FILE* fp2) {
	int n;
	char c;
	fscanf(fp1, "%c%d", &c, &n);
	if (n != 1) return;
	for (int k = 1; k <= 81; k++) {
		if (!((k - 1) % 9)) fprintf(fp2, "\n");
		fscanf(fp1, " %c", &c);
		for (int i = 1; i <= 9; i++) {
			fscanf(fp1, "%d", &n);
			if (n > 0) fprintf(fp2, "%3d", i);
		}
	}
}
void Sudplay() {
	int choice = 1, diff, op, x, y, v;
	while (choice) {
		system("cls");
		printf("请选择难度：\n");
		printf("   1.简单     2.中等     3.困难     4.噩梦\n");
		scanf("%d", &diff);
		printf("正在生成数独，请稍等");
		sud* k;
		k = Sudcreate();
		for (int i = 1; i <= (4 - diff) * 10; i++) {
			hint(k);
		}
		op = 1;
		while (op) {
			system("cls");
			printf("------------sudoku------------\n\n");
			for (int i = 1; i <= 9; i++) {
				for (int j = 1; j <= 9; j++) {
					printf("%3d", k->a[i][j]);
				}
				printf("\n");
			}
			printf("------------------------------\n");
			printf("\n1.填写        2.更多提示\n0.显示答案\n");
			scanf("%d", &op);
			if (op == 2) hint(k);
			if (op == 1) {
				printf("\n输入填写的坐标与待填值:");
				scanf("%d%d%d", &x, &y, &v);
				if (!k->a[x][y] && v == k->ori[x][y]) {
					printf("\n填写正确!\n");
					k->a[x][y] = k->ori[x][y];
					getchar(); getchar();
				}
				else {
					printf("\n答案错误\n");
					getchar(); getchar();
				}
			}
		}
		ShowAns(k);
		printf("1.再来一局       0.退出\n");
		free(k);
		scanf("%d", &choice);
	}
}
void hint(sud* k) {
	int x, y, fg;
	fg = 1;
	srand(time(NULL));
	while (fg) {//为空
		x = rand() % 9 + 1;
		y = rand() % 9 + 1;
		if (!k->a[x][y]) {
			k->a[x][y] = k->ori[x][y];
			fg = 0;
		}
	}
}