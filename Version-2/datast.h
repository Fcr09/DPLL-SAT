#pragma once
#define MAX_STATENUM 25000
#define MAX_VARNUM 3000
#define ERROR 0
#define OK 1
typedef struct statement {
	int num;                    //标记单句的个数
	struct node* firstnode;     //第一个变量
	struct statement* next;     //下一子句
}st;
typedef struct node {
	int val;
	struct node* next;
}node;
struct problem {
	int varnum, statenum;       //变量数量、语句数量
	struct statement* root;     //root结点中不存放数据
};
struct count {
	int num = 0;
	int pnum = 0;
	int nnum = 0;
};
struct sud {
	int ori[10][10];            //记录原始数值
	int a[10][10];              //0表示挖出的洞，其他情况同ori
	int num;                    //非0个数
};