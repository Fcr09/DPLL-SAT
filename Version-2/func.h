#pragma once
#include "datast.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
void DPLL();                                            //DPLL�������
void init(problem* P);                                  //��ʼ��root���
void show(problem* P);                                  //��ӡP����Ч�Ӿ�
void read(FILE* fp, problem* P);                        //��cnf�ļ���������
int FindUnit(problem* p);                               //Ѱ�ҵ��Ӿ�
int DeleteUnit(problem* P, int unit);                   //���õ��Ӿ���򻯼�P
void AddUnit(problem* P, int unit);                     //�ײ巨���ӵ��Ӿ�
int select(problem* P);                                 //������ѡ�����
void sfree(st* s);                                      //�ͷ��Ӿ�
void pfree(problem* P);                                 //��������
int solve(problem* P);                                 //�ݹ���⺯��
void Ansprint(FILE* fp, problem* P, int succ, int t);   //�����
int minselect(problem* P);                              //����Ӿ����
problem* PCopy(problem* P1);

int VarTrans(int i, int j, int n);                      //������Ȼ������cnf����ת��
void BasicPrint(FILE* fp);                              //�������������cnf���
void ShowSud(FILE* fp);                                 //��res�ļ��������������
void SudToSat(char FileName[]);                         //������sat�����ת��
void Sudsolve();                                        //�����֪��ֵ�����
void Sudprint(FILE* fp, sud* k);                        //�������ļ�д��
void Sudread(FILE* fp, sud* k);                         //�������ļ���ȡ
void Sudplay();                                         //��������
void Candig(sud* k, int x, int y);                      //��������ڶ�  
void DigHole(sud* k);                                   //�����ڶ�
void ShowAns(sud* k);                                   //���������
sud* Sudcreate();                                       //�����������
void fShowSud(FILE* fp1, FILE* fp2);                    //res�ļ��������ļ�ת��
void hint(sud* k);                                      //�����ʾ
