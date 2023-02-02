#include "datast.h"
#include "func.h"
void DPLL() {
	problem *P = (problem*)malloc(sizeof(problem));
	char FileName[20];
	printf("����������ļ���\n");	
	FILE* fp=NULL;
	while (!fp) {
		scanf("%s",FileName);
		fp = fopen(FileName, "r");
		if (!fp) printf("��ʧ�ܣ������ļ�����������\n");
	}
	read(fp,P);
	fclose(fp);
	int choice,state,flag,len;
	char c;
	flag = 1;
	while (flag) {
		system("cls");
		printf("�ļ�����ɹ���\n");
		printf("1.��֤��ȡ���   2.cnf���\n");
		printf("0.�˳�\n");
		scanf("%d", &choice);
		switch (choice) {
		case 1:
			system("cls");
			show(P);//��֤����
			c = getchar(); c = getchar();
			break;
		case 2:
			system("cls");
			time_t t1, t2;
			t1 = clock();
			state = solve(P, 1, 0);//�������
			t2 = clock();
			recovery(P, 1);
			if (state == OK) {
				printf("������!\n");
				for (int i = 1; i <= P->varnum; i++) {//��stdout��ʾ
					if (P->ans[i]) printf("%5d ", i);
					else printf("%5d ", (-1) * i);
					if (!(i % 15)) printf("\n");
				}			
			}
			else printf("��������!");
			printf("\n��ʱ: %d ms", t2 - t1);

			len = strlen(FileName);
			FileName[len - 3] = 'r', FileName[len - 2] = 'e', FileName[len - 1] = 's';
			fp = fopen(FileName,"w+");
			Ansprint(fp, P, state, t2 - t1);//����res�ļ�
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
	printf("��ȡ�ɹ�\n����%d����Ԫ��%d���Ӿ�\n",P->varnum,P->statenum);
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
	while ((c = fgetc(fp)) == 'c') {//��ȡע��
		ungetc(c,fp);
		fscanf(fp, "%[^\n]", r);
		c=fgetc(fp);
	}
	fscanf(fp,"%s%d%d",r,&P->varnum,&P->statenum);
	init(P);
	st* s = P->root;
	node* p;
	for (int i = 1; i <= P->statenum; i++) {//�����ڽӱ�
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
		while (var) {//�ǽ�����
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
int FindUnit(problem *P) {//Ѱ�ҵ��Ӿ�
	int res = 0;
	st* s=P->root;
	node* p;
	while (s->next) {//��������
		s = s->next;
		if (s->mark) continue;
		if (s->num==1) {//�ҵ�s->num==1�ľ����е���Ч����
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
	while (s->next) {//��������
		s = s->next;
		if (s->mark) continue;
		p = s->firstnode;
		while (p) {//���������еı�Ԫ
			if (p->mark) {
				p = p->next;
				continue;
			}
			if (p->val == unit) {//ɾ������
				s->mark = depth;//�õݹ���ȱ�ǽ���Ա����
				p = s->firstnode;
				while (p) {//������ǰ�����еı�Ԫ
					if (p->mark) {
						p = p->next;
						continue;
					}
					P->cnt[abs(p->val)]--;//�޸ı�Ԫ���ִ���
					p = p->next;
				}
				break;
			}
			if (p->val + unit == 0) {//ɾ����Ԫ
				p->mark = depth;
				s->num--;//�޸ĵ�ǰ���ӵı�Ԫ��
				P->cnt[abs(p->val)]--;//�޸ı�Ԫ���ִ���
				if (!s->num) //���޸ĺ�ǰ����Ϊ�գ���˵����ͻ
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
	while (s->next) {//��������
		s = s->next;
		if (s->mark == depth) {//��������depth��ȱ�ɾ��
			s->mark = 0;//�ָ���Ч��
			p = s->firstnode;
			while (p) {//���������еı�Ԫ
				if (p->mark) {
					if (p->mark == depth) {
						p->mark = 0;
						s->num++;
						P->cnt[abs(p->val)]++;//�޸ĳ��ִ���
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
		while (p) {//��������
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
void sfree(st* s) {//�ͷž���
	node* p1 = s->firstnode, * p2;
	while (p1) {
		p2 = p1->next;
		free(p1);
		p1 = p2;
	}
	free(s);
}
void pfree(problem* P) {//�ͷ�����
	st* s1 = P->root,*s2;
	while (s1) {
		s2 = s1->next;
		sfree(s1);
		s1 = s2;
	}
	free(P);
}
int select(problem *P) {//ѡ����ִ������ı���
	int max = 0, res = 0;
	for (int i = 1; i <= MAX_VARNUM; i++) {
		if (max < P->cnt[i]) {
			max = P->cnt[i];
			res = i;
		}
	}
	return res;
}
void AddUnit(problem *P, int unit) {//�ײ巨����ֻ��һ������unit�ľ���
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
void deletelast(problem* P, int sup) {//ɾ��֮ǰ�ײ�ľ���
	st* s1 = P->root,*s2;
	s2 = s1->next;
	s1->next = s2->next;
	P->cnt[abs(sup)]--;
	sfree(s2);
	P->statenum--;
}
int solve(problem* P,int depth,int clue) {
	int sig;
	if (!clue) sig = FindUnit(P);//����������Ѱ�ҵ��Ӿ�
	else sig = clue;//����Ϊ���Ӿ��Ԫ��
	while (sig) {//�����ҵ����Ӿ�
		if (sig > 0) P->ans[sig] = true;//�𰸸�ֵ
		else P->ans[(-1)*sig] = false;
		int state = DeleteUnit(P, sig, depth);//���õ��Ӿ���򻯼�����
		if (state==ERROR) {
			return ERROR;
		}
		sig = FindUnit(P);//����Ѱ�ҵ��Ӿ�
	}
	int sup = select(P);//ѡȡ���ִ������ı�Ԫ
	if (!sup) return OK;//���ޱ�Ԫ��˵��������
	AddUnit(P, sup);//����ѡȡ�ı�ԪΪ��
	if (solve(P, depth + 1,sup)) {//�����ļ����Ϊ���Ӿ䣬������Ϊ����
		recovery(P, depth + 1);//�ָ�depth+1�ݹ�����������޸�
		deletelast(P, sup);//ɾ������
		return OK;
	}
	recovery(P, depth + 1);//�ָ�depth+1�ݹ���������޸�
	deletelast(P,sup);//ɾ������
	AddUnit(P, sup * (-1));//����ѡȡ�ı�ԪΪ��
	if (solve(P, depth + 1,(-1)*sup)) {
		recovery(P, depth + 1);
		deletelast(P, sup);
		return OK;
	}
	recovery(P, depth + 1);
	deletelast(P,sup);
	return ERROR;//������֧��Ϊ�٣�����Ϊ��
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