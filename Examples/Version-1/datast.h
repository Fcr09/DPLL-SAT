#pragma once
#define MAX_STATENUM 25000
#define MAX_VARNUM 3000
#define ERROR 0
#define OK 1
typedef struct statement {
	int mark;                   //mark=0��ʾ�Ӿ���Ч,���������ʾɾ��ʱ�ĵݹ����
	int num;                    //��ǵ���ĸ���
	struct node* firstnode;     //��һ������
	struct statement* next;     //��һ�Ӿ�
}st;
typedef struct node {
	int mark;                   //mark=0��ʾ������Ч,���������ʾɾ��ʱ�ĵݹ����
	int val;
	struct node* next;
}node;
struct problem {
	int varnum, statenum;       //�����������������
	struct statement* root;     //root����в��������
	int cnt[MAX_VARNUM + 1];    //��¼ÿ���������ֵĴ���
	bool ans[MAX_VARNUM + 1];   //��¼��
};
struct sud {
	int ori[10][10];            //��¼ԭʼ��ֵ
	int a[10][10];              //0��ʾ�ڳ��Ķ����������ͬori
	int num;                    //��0����
};