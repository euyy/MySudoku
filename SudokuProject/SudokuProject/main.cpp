#include"generator.h"
#include"solver.h"
#include"common.h"
#include<time.h>
#define CLOCKS_PER_SEC  ((clock_t)1000)
using namespace std;

int main(int argc, char *argv[])
{

	if (argc == 3 && strcmp(argv[1], "-c") == 0)
	{
		int len = strlen(argv[2]);
		int finals_num = 0;
		int flag = 0;
		for (int i = 0; i < len; i++)
		{
			if (argv[2][i] <= '9'&&argv[2][i] >= '0')
				finals_num = finals_num * 10 + argv[2][i] - '0';
			else {
				cout << "Input Number Error!" << endl;
				flag = 1;
				break;
			}
		}
		if (flag == 0) {
			//cout << "finals number is:" << finals_num << endl;
			Generate_Sudoku(finals_num);
		}
	}
	else if (argc == 3 && strcmp(argv[1], "-s") == 0)
	{
		Solve_Sudoku(argv[2]);
	}
	else {
		cout << "Input Error!" << endl;
	}

	cout << "Totle Time : " << (double)clock() / CLOCKS_PER_SEC << "s" << endl;
	system("pause");
	return 0;
}