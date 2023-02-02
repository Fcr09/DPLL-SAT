#include "func.h"
#include "datast.h"

int main() {
	int choice = 1;
	while (choice) {//打印菜单
		system("cls");
		printf("                  Menu for SAT Solver based on DPLL                  \n");
		printf("---------------------------------------------------------------------\n");
		printf("          1.SAT        2.Suduku Solve       3.Suduku Play            \n");
		printf("          0.Exit                                                     \n");
		printf("---------------------------------------------------------------------\n");
		scanf("%d", &choice);//读取选项
		switch (choice) {
		case 1:
			DPLL();
			break;
		case 2:
			Sudsolve();
			getchar(); getchar();
			break;
		case 3:
			Sudplay();
			getchar(); getchar();
		default:
			break;
		}
	}
}
