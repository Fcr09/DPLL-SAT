#pragma once
#include "datast.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
void DPLL();                                            //DPLL问题求解
void init(problem* P);                                  //初始化root结点
void show(problem* P);                                  //打印P中有效子句
void read(FILE* fp, problem* P);                        //由cnf文件读入问题
int FindUnit(problem* p);                               //寻找单子句
int DeleteUnit(problem* P, int unit);                   //利用单子句规则化简P
void AddUnit(problem* P, int unit);                     //首插法增加单子句
int select(problem* P);                                 //最多变量选择策略
void sfree(st* s);                                      //释放子句
void pfree(problem* P);                                 //销毁问题
int solve(problem* P);                                 //递归求解函数
void Ansprint(FILE* fp, problem* P, int succ, int t);   //输出答案
int minselect(problem* P);                              //最短子句策略
problem* PCopy(problem* P1);

int VarTrans(int i, int j, int n);                      //数独自然编码与cnf编码转化
void BasicPrint(FILE* fp);                              //数独基本规则的cnf输出
void ShowSud(FILE* fp);                                 //由res文件解码读出数独答案
void SudToSat(char FileName[]);                         //数独向sat问题的转化
void Sudsolve();                                        //求解已知格局的数独
void Sudprint(FILE* fp, sud* k);                        //数独的文件写入
void Sudread(FILE* fp, sud* k);                         //数独的文件读取
void Sudplay();                                         //数独游玩
void Candig(sud* k, int x, int y);                      //单个点的挖洞  
void DigHole(sud* k);                                   //数独挖洞
void ShowAns(sud* k);                                   //数独答案输出
sud* Sudcreate();                                       //数独格局生成
void fShowSud(FILE* fp1, FILE* fp2);                    //res文件向数独文件转化
void hint(sud* k);                                      //随机提示
