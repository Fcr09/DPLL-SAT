#pragma once
#define MAX_STATENUM 25000
#define MAX_VARNUM 3000
#define ERROR 0
#define OK 1
typedef struct statement {
	int num;                    //��ǵ���ĸ���
	struct node* firstnode;     //��һ������
	struct statement* next;     //��һ�Ӿ�
}st;
typedef struct node {
	int val;
	struct node* next;
}node;
struct problem {
	int varnum, statenum;       //�����������������
	struct statement* root;     //root����в��������
};
struct count {
	int num = 0;
	int pnum = 0;
	int nnum = 0;
};
struct sud {
	int ori[10][10];            //��¼ԭʼ��ֵ
	int a[10][10];              //0��ʾ�ڳ��Ķ����������ͬori
	int num;                    //��0����
};