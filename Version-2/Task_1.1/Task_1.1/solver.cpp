#include "datast.h"
#include "func.h"
bool ans[MAX_VARNUM];
void DPLL() {
	problem* P = (problem*)malloc(sizeof(problem));
	char FileName[25];
	printf("请输入测试文件名\n");
	FILE* fp = NULL;
	while (!fp) {
		scanf("%s", FileName);
		fp = fopen(FileName, "r");
		if (!fp) printf("打开失败，请检查文件并重新输入\n");
	}
	read(fp, P);
	fclose(fp);
	int choice, state, flag, len;
	char c;
	flag = 1;
	while (flag) {
		system("cls");
		printf("文件读入成功！\n");
		printf("1.验证读取结果   2.cnf求解\n");
		printf("0.退出\n");
		scanf("%d", &choice);
		switch (choice) {
		case 1:
			system("cls");
			show(P);//验证读入
			c = getchar(); c = getchar();
			break;
		case 2:
			system("cls");
			time_t t1, t2;
			t1 = clock();
			state = solve(P);//求解问题
			t2 = clock();
			if (state == OK) {
				printf("可满足!\n");
				for (int i = 1; i <= P->varnum; i++) {//stdout显示答案
					if (ans[i]) printf("%5d ", i);
					else printf("%5d ", (-1) * i);
					if (!(i % 15)) printf("\n");
				}
			}
			else printf("不可满足!");
			printf("\n用时: %d ms", t2 - t1);

			len = strlen(FileName);
			FileName[len - 3] = 'r', FileName[len - 2] = 'e', FileName[len - 1] = 's';
			fp = fopen(FileName, "w+");
			Ansprint(fp, P, state, t2 - t1);//res文件输出
			fclose(fp);

			c = getchar(); c = getchar();
			break;
		default:
			flag = 0;
		}
	}
	pfree(P);
}
void init(problem* P) {
	P->root = (st*)malloc(sizeof(st));
	P->root->firstnode = NULL;
	P->root->next = NULL;
	P->root->num = 0;
	return;
}
void show(problem* P) {
	st* s = P->root;
	node* p;
	system("cls");
	printf("读取成功\n共有%d个变元，%d个子句\n", P->varnum, P->statenum);
	while (s->next) {//遍历子句集
		s = s->next;
		p = s->firstnode;
		while (p) {
			printf("%d ", p->val);
			p = p->next;
		}
		printf("0\n");
	}
	return;
}
void read(FILE* fp, problem* P) {
	char c, r[500];
	int var;
	while ((c = fgetc(fp)) == 'c') {//读取注释
		ungetc(c, fp);
		fscanf(fp, "%[^\n]", r);
		c = fgetc(fp);
	}
	fscanf(fp, "%s%d%d", r, &P->varnum, &P->statenum);
	init(P);
	st* s = P->root;
	node* p;
	for (int i = 1; i <= P->statenum; i++) {//建立邻接表
		s->next = (st*)malloc(sizeof(st));
		s = s->next;
		s->num = 0;
		p = (node*)malloc(sizeof(node));
		fscanf(fp, "%d", &p->val);
		s->num++;
		s->firstnode = p;
		fscanf(fp, "%d", &var);
		while (var) {//非结束符
			p->next = (node*)malloc(sizeof(node));
			p = p->next;
			p->val = var;
			s->num++;
			fscanf(fp, "%d", &var);
		}
		p->next = NULL;
	}
	s->next = NULL;
}
int FindUnit(problem* P) {//寻找单子句
	int res = 0;
	st* s = P->root;
	while (s->next) {//遍历句子
		s = s->next;
		if (s->num == 1) {//找到s->num==1的句子中的有效变量
			res = s->firstnode->val;
			break;
		}
	}
	return res;
}
int DeleteUnit(problem* P, int unit) {
	st* s1 = P->root,*s2;
	node* p1,*p2;
	while (s1->next) {//遍历句子
		s2 = s1->next;
		p1 = s2->firstnode;
		if (p1->val == unit) {
			s1->next = s2->next;
			p1 = s2->firstnode;
			while (p1) {
				p1 = p1->next;
			}
			sfree(s2);
			continue;
		}
		while (p1->val + unit == 0) {//特判首结点
			s2->firstnode = p1->next;
			free(p1);
			p1 = s2->firstnode;
			s2->num--;
			if (!p1) return ERROR;
		}
		while (p1->next) {
			p2 = p1->next;
			if (p2->val == unit) {//删除子句
				s1->next = s2->next;
				p1 = s2->firstnode;
				sfree(s2);
				s2 = s1;
				break;
			}
			if (p2->val + unit == 0) {//删除结点
				p1->next = p2->next;
				free(p2);
				s2->num--;
				continue;
			}
			p1 = p2;
		}
		s1 = s2;
	}
	return OK;
}
void sfree(st* s) {//释放句子
	node* p1 = s->firstnode, * p2;
	while (p1) {
		p2 = p1->next;
		free(p1);
		p1 = p2;
	}
	free(s);
}
void pfree(problem* P) {//释放问题
	st* s1 = P->root, * s2;
	while (s1) {
		s2 = s1->next;
		sfree(s1);
		s1 = s2;
	}
	free(P);
}

void AddUnit(problem* P, int unit) {//首插法插入只含一个变量unit的句子
	st* s = P->root;
	st* s1 = s->next, * s2;
	s2 = (st*)malloc(sizeof(st));
	s2->next = s1;
	s->next = s2;
	s2->num = 1;
	node* p = (node*)malloc(sizeof(node));
	p->val = unit;
	p->next = NULL;
	s2->firstnode = p;
	P->statenum++;
}
int solve(problem* P) {
	int sig;
	sig = FindUnit(P);//寻找单子句
	while (sig) {//若能找到单子句
		if (sig > 0) ans[sig] = true;//答案赋值
		else ans[(-1) * sig] = false;
		int state = DeleteUnit(P, sig);//利用单子句规则化简问题
		if (state == ERROR) {
			return ERROR;
		}
		sig = FindUnit(P);//继续寻找单子句
	}	
	//int sup = select(P);//选取出现次数最多的变元
	//if (!sup) return OK;//若无变元，说明求解完毕
	if (!P->root->next) return OK;
	int sup = P->root->next->firstnode->val;
	problem* P1,*P2;
	P1 = PCopy(P);
	AddUnit(P1, sup);
	if (solve(P1)) {
		return OK;
	}
	pfree(P1);
	P2 = PCopy(P);
	AddUnit(P2, -1 * sup);
	if (solve(P2)) {
		return OK;
	}
	pfree(P2);
	return ERROR;//若两分支都为假，则结果为假
}
problem* PCopy(problem* P1) {
	problem* P2 = (problem*)malloc(sizeof(problem));//新建副本
	P2->varnum = P1->varnum;//赋值变元数和子句数
	P2->statenum = P1->statenum;
	st* s1, * s2;
	node* p1, * p2;
	s1 = P1->root;
	s2 = (st*)malloc(sizeof(st));	
	s2->firstnode = NULL;
	s2->num = 0;
	P2->root = s2;
	while (s1->next) {//子句链表复制
		s1 = s1->next;
		s2->next = (st*)malloc(sizeof(st));
		s2 = s2->next;
		s2->num = s1->num;
		p1 = s1->firstnode;
		s2->firstnode = (node*)malloc(sizeof(node));//特判首结点
		p2 = s2->firstnode;
		p2->val = p1->val;
		while (p1->next) {//元素链表复制
			p1 = p1->next;
			p2->next = (node*)malloc(sizeof(node));
			p2 = p2->next;
			p2->val = p1->val;
		}
		p2->next = NULL;
	}
	s2->next = NULL;
	return P2;
}
void Ansprint(FILE* fp, problem* P, int succ, int t) {
	fprintf(fp, "s %d", succ);
	if (succ != 1) return;
	for (int i = 1; i <= P->varnum; i++) {
		if (!((i - 1) % 9)) fprintf(fp, "\nv");
		if (ans[i]) fprintf(fp, "%5d ", i);
		else fprintf(fp, "%5d ", (-1) * i);
	}
	fprintf(fp, "\nt %d", t);
}

int select(problem* P) {//选择出现次数最多的变量
	count* cnt = new count[P->varnum+1];
	st* s = P->root;
	node* p;
	while (s->next) {
		s = s->next;
		p = s->firstnode;
		while (p) {
			cnt[abs(p->val)].num++;
			if (p->val > 0) cnt[abs(p->val)].pnum++;
			else cnt[abs(p->val)].nnum++;
			p = p->next;
		}
	}
	int fg = 0, res = 0, max = 0;
	for (int i = 1; i <= P->varnum; i++) {
		if (max < cnt[i].num) {
			max = cnt[i].num;
			res = i;
			if (cnt[i].pnum > cnt[i].nnum) fg = 1;
			else fg = -1;
		}
	}
	delete[] cnt;
	return fg * res;
}

int minselect(problem* P) {
	st* s = P->root;
	node* p;
	int min;
	min = 0xFFFFFF, p = NULL;
	while (s->next) {
		s = s->next;
		if (s->num < min) {
			min = s->num;
			p = s->firstnode;
		}
	}
	if (!p) return 0;
	return p->val;
}
