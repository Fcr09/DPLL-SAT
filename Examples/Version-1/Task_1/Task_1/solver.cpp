#include "datast.h"
#include "func.h"
void DPLL() {
	problem *P = (problem*)malloc(sizeof(problem));
	char FileName[20];
	printf("请输入测试文件名\n");	
	FILE* fp=NULL;
	while (!fp) {
		scanf("%s",FileName);
		fp = fopen(FileName, "r");
		if (!fp) printf("打开失败，请检查文件并重新输入\n");
	}
	read(fp,P);
	fclose(fp);
	int choice,state,flag,len;
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
			state = solve(P, 1, 0);//求解问题
			t2 = clock();
			recovery(P, 1);
			if (state == OK) {
				printf("可满足!\n");
				for (int i = 1; i <= P->varnum; i++) {//答案stdout显示
					if (P->ans[i]) printf("%5d ", i);
					else printf("%5d ", (-1) * i);
					if (!(i % 15)) printf("\n");
				}			
			}
			else printf("不可满足!");
			printf("\n用时: %d ms", t2 - t1);

			len = strlen(FileName);
			FileName[len - 3] = 'r', FileName[len - 2] = 'e', FileName[len - 1] = 's';
			fp = fopen(FileName,"w+");
			Ansprint(fp, P, state, t2 - t1);//生成res文件
			fclose(fp);

			c = getchar(); c = getchar();
			break;
		default:
			flag = 0;
		}
	}
	pfree(P);
}
void init(problem *P) {
	P->root = (st*)malloc(sizeof(st));
	P->root->firstnode = NULL;
	P->root->next = NULL;
	P->root->mark = 0;
	P->root->num = 0;
	memset(P->ans, 0, sizeof(P->ans));
	memset(P->cnt, 0, sizeof(P->cnt));
	return;
}
void show(problem *P) {
	st* s=P->root;
	node* p;
	system("cls");
	printf("读取成功\n共有%d个变元，%d个子句\n",P->varnum,P->statenum);
	while (s->next) {	
		s = s->next;
		if (s->mark) continue;
		p = s->firstnode;
		while (p) {
			if(!p->mark) printf("%d ",p->val);
			p = p->next;
		}
		printf("0\n");
	}
	return;
}
void read(FILE* fp,problem *P) {
	char c,r[500];
	int var;
	while ((c = fgetc(fp)) == 'c') {//读取注释
		ungetc(c,fp);
		fscanf(fp, "%[^\n]", r);
		c=fgetc(fp);
	}
	fscanf(fp,"%s%d%d",r,&P->varnum,&P->statenum);
	init(P);
	st* s = P->root;
	node* p;
	for (int i = 1; i <= P->statenum; i++) {//建立邻接表
		s->next = (st*)malloc(sizeof(st));
		s = s->next;
		s->num = 0;
		s->mark = 0;
		p = (node*)malloc(sizeof(node));
		fscanf(fp,"%d",&p->val);
		p->mark = 0;
		s->num++;
		P->cnt[abs(p->val)]++;
		s->firstnode = p;
		fscanf(fp,"%d",&var);
		while (var) {//非结束符
			p->next = (node*)malloc(sizeof(node));
			p = p->next;
			p->mark = 0;
			p->val = var;
			s->num++;
			P->cnt[abs(var)]++;
			fscanf(fp, "%d", &var);
		}
		p->next = NULL;
	}
	s->next = NULL;
}
int FindUnit(problem *P) {//寻找单子句
	int res = 0;
	st* s=P->root;
	node* p;
	while (s->next) {//遍历句子
		s = s->next;
		if (s->mark) continue;
		if (s->num==1) {//找到s->num==1的句子中的有效变量
			p = s->firstnode;
			while (p && p->mark) {
				p = p->next;
			}
			if (p) {
				res = p->val;
				break;
			}
		}
	}
	return res;
}
int DeleteUnit(problem* P, int unit,int depth) {
	st* s=P->root;
	node* p;
	while (s->next) {//遍历句子
		s = s->next;
		if (s->mark) continue;
		p = s->firstnode;
		while (p) {//遍历句子中的变元
			if (p->mark) {
				p = p->next;
				continue;
			}
			if (p->val == unit) {//删除句子
				s->mark = depth;//用递归深度标记结点以便回溯
				p = s->firstnode;
				while (p) {//遍历当前句子中的变元
					if (p->mark) {
						p = p->next;
						continue;
					}
					P->cnt[abs(p->val)]--;//修改变元出现次数
					p = p->next;
				}
				break;
			}
			if (p->val + unit == 0) {//删除变元
				p->mark = depth;
				s->num--;//修改当前句子的变元数
				P->cnt[abs(p->val)]--;//修改变元出现次数
				if (!s->num) //若修改后当前句子为空，则说明冲突
					return ERROR;
			}
			p = p->next;
		}
	}
	return OK;
}
void recovery(problem* P, int depth) {
	st* s = P->root;
	node* p;
	while (s->next) {//遍历句子
		s = s->next;
		if (s->mark == depth) {//若句子在depth深度被删除
			s->mark = 0;//恢复有效性
			p = s->firstnode;
			while (p) {//遍历句子中的变元
				if (p->mark) {
					if (p->mark == depth) {
						p->mark = 0;
						s->num++;
						P->cnt[abs(p->val)]++;//修改出现次数
					}
					p = p->next;
					continue;
				}
				P->cnt[abs(p->val)]++;
				p = p->next;
			}
			continue;
		}
		p = s->firstnode;
		while (p) {//遍历变量
			if (p->mark != depth) {
				p = p->next;
				continue;
			}
			p->mark = 0;
			s->num++;
			P->cnt[abs(p->val)]++;
			p = p->next;
		}
	}
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
	st* s1 = P->root,*s2;
	while (s1) {
		s2 = s1->next;
		sfree(s1);
		s1 = s2;
	}
	free(P);
}
int select(problem *P) {//选择出现次数最多的变量
	int max = 0, res = 0;
	for (int i = 1; i <= MAX_VARNUM; i++) {
		if (max < P->cnt[i]) {
			max = P->cnt[i];
			res = i;
		}
	}
	return res;
}
void AddUnit(problem *P, int unit) {//首插法插入只含一个变量unit的句子
	st* s = P->root;
	st* s1 = s->next,*s2;
	s2 = (st*)malloc(sizeof(st));
	s2->next = s1;
	s->next = s2;
	s2->mark = 0;
	s2->num = 1;
	node* p = (node*)malloc(sizeof(node));
	p->val = unit;
	P->cnt[abs(unit)]++;
	p->next = NULL;
	p->mark = 0;
	s2->firstnode = p;
	P->statenum++;
}
void deletelast(problem* P, int sup) {//删除之前首插的句子
	st* s1 = P->root,*s2;
	s2 = s1->next;
	s1->next = s2->next;
	P->cnt[abs(sup)]--;
	sfree(s2);
	P->statenum--;
}
int solve(problem* P,int depth,int clue) {
	int sig;
	if (!clue) sig = FindUnit(P);//若无线索，寻找单子句
	else sig = clue;//线索为单子句的元素
	while (sig) {//若能找到单子句
		if (sig > 0) P->ans[sig] = true;//答案赋值
		else P->ans[(-1)*sig] = false;
		int state = DeleteUnit(P, sig, depth);//利用单子句规则化简问题
		if (state==ERROR) {
			return ERROR;
		}
		sig = FindUnit(P);//继续寻找单子句
	}
	int sup = select(P);//选取出现次数最多的变元
	if (!sup) return OK;//若无变元，说明求解完毕
	AddUnit(P, sup);//假设选取的变元为真
	if (solve(P, depth + 1,sup)) {//新增的假设必为单子句，将其作为线索
		recovery(P, depth + 1);//恢复depth+1递归深度所做的修改
		deletelast(P, sup);//删除假设
		return OK;
	}
	recovery(P, depth + 1);//恢复depth+1递归深度所做修改
	deletelast(P,sup);//删除假设
	AddUnit(P, sup * (-1));//假设选取的变元为假
	if (solve(P, depth + 1,(-1)*sup)) {
		recovery(P, depth + 1);
		deletelast(P, sup);
		return OK;
	}
	recovery(P, depth + 1);
	deletelast(P,sup);
	return ERROR;//若两分支都为假，则结果为假
}
void Ansprint(FILE* fp, problem *P, int succ, int t) {
	fprintf(fp,"s %d",succ);
	if (succ != 1) return;
	for (int i = 1; i <= P->varnum; i++) {
		if (!((i - 1) % 9)) fprintf(fp,"\nv");
		if (P->ans[i]) fprintf(fp,"%5d ", i);
		else fprintf(fp,"%5d ", (-1) * i);
	}
	fprintf(fp,"\nt %d",t);	
}
int momselect(problem* P) {
	st* s = P->root;
	node* p;
	int min,fg,res;
	fg = 1;
	min = 0xFFFFFF, p = NULL;
	while (s->next) {
		s = s->next;
		if (s->mark) continue;
		if (s->num && s->num < min) {
			min = s->num;
			fg = 0;
		}
	}
	if (fg) return 0;	
	memset(P->cnt, 0, sizeof(P->cnt));
	s = P->root;
	while (s->next) {
		s = s->next;
		if (s->num != min||s->mark) continue;
		p = s->firstnode;
		while (p) {
			if (p->mark) {
				p = p->next;
				continue;
			}
			P->cnt[abs(p->val)]++;
			p = p->next;
		}
	}
	int max = 0;
	res = 0;
	for (int i = 1; i <= P->varnum; i++) {
		if (max < P->cnt[i]) {
			max = P->cnt[i];
			res = i;
		}
	}
	return res;
}
int minselect(problem* P) {
	st* s = P->root;
	node* p;
	int min;
	min = 0xFFFFFF, p = NULL;
	while (s->next) {
		s = s->next;
		if (s->mark) continue;
		if (s->num && s->num < min) {
			min = s->num;
			p = s->firstnode;
		}
	}
	if (!p) return 0;
	min = 0xFFFFFF;
	while (p) {
		if (p->mark) {
			p = p->next;
			continue;
		}
		return p->val;
	}
	return 0;
}