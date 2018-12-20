#include"solver.h"
struct Puzzles {
	int sudoku[9][9];//数独谜题
	int map[9][9];//每个格子有几个数可以填写
	int row[9];//每一行有哪些数可以填写
	int col[9];//每一列有哪些数可以填写
	int matrix[3][3];//每一个3*3方阵有哪些数可以填写
	int empty;//还有几个未填写的格子
};

int bit[10] = { 0,1,2,4,8,16,32,64,128,256 };

void Init(Puzzles &puz)
{
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			puz.map[i][j] = 9;
			puz.sudoku[i][j] = 0;
			if (i < 3 && j < 3)puz.matrix[i][j] = 511;
		}
		puz.row[i] = 511;
		puz.col[i] = 511;
	}
	puz.empty = 81;
}
		
Puzzles ReadPuzzle(FILE* fr)
{
	Puzzles puz;
	char ch;
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			int matr_i = i / 3;
			int matr_j = j / 3;
			fscanf(fr, "%d%c", &puz.sudoku[i][j],&ch);
			puz.row[i] -= puz.sudoku[i][j];
			puz.col[j] -= puz.sudoku[i][j];
			puz.matrix[matr_i][matr_j] -= puz.sudoku[i][j];
			if (puz.sudoku[i][j] > 0) {
				puz.map[i][j] = 0;
				puz.empty--;
			}
		}
	}
	return puz;
}

void Solve(Puzzles &puz)
{

}

void Solve_Sudoku()
{
	FILE *fr = fopen("SudokuQuestions.txt", "r");
	if (fr == NULL)
	{
		cout << "Open File SudokuQuestions.txt Failed!" << endl;
		return;
	}

	Puzzles puz;
	Init(puz);
	do {
		puz = ReadPuzzle(fr);
		Solve(puz);
		
	} while (fgetc(fr) != EOF);
	
}
