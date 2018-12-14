#include"generator.h"
#include"solver.h"
#include<iostream>
#include<string.h>
using namespace std;

int main(int argc, char *argv[])
{
	
	if (argc == 3 && strcmp(argv[1], "-c") == 0)
	{
		int len = strlen(argv[2]);
		int finals_num = 0;

		for (int i = 0; i < len; i++)
		{
			if (argv[2][i] >= '0'&&argv[2][i] <= '9')
			{
				finals_num = finals_num * 10 + (argv[2][i] - '0');
			}
			else
			{
				printf("Input Error!\n");
				return 0;
			}
		}

		Generate_Sudoku(finals_num);

	}
	else if (argc == 3 && strcmp(argv[1], "-s") == 0)
	{
		Solve_Sudoku();
		
	}
	else {
		cout << "Error!" << endl;
	}

}