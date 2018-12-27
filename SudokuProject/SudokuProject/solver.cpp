#pragma once
#include"solver.h"

const int FALSE = 0;
const int TRUE = 1;

char result_set[163000000];
int cnt1 = 0;
Puzzles result;

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

void Generate_Map(Puzzles &puz)
{
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			if (puz.sudoku[i][j] == 0) {
				int matr_i = i / 3;
				int matr_j = j / 3;
				puz.map[i][j] = puz.row[i] & puz.col[j] & puz.matrix[matr_i][matr_j];
			}
			else puz.map[i][j] = 0;
		}
	}
}

Puzzles ReadPuzzle(FILE* fr)
{
	Puzzles puz;
	Init(puz);
	char str[20];

	for (int i = 0; i < 9; i++)
	{
		fgets(str, 20, fr);
		for (int j = 0; j < 9; j++)
		{
			int matr_i = i / 3;
			int matr_j = j / 3;
			puz.sudoku[i][j] = str[j * 2] - '0';
			//fscanf(fr, "%d%c", &puz.sudoku[i][j], &ch);
			puz.row[i] -= bit[puz.sudoku[i][j]];
			puz.col[j] -= bit[puz.sudoku[i][j]];
			puz.matrix[matr_i][matr_j] -= bit[puz.sudoku[i][j]];
			if (puz.sudoku[i][j] > 0) {
				puz.map[i][j] = 0;
				puz.empty--;
			}
		}
	}

	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			if (puz.sudoku[i][j] == 0)
			{
				int matr_i = i / 3;
				int matr_j = j / 3;
				puz.map[i][j] = puz.row[i] & puz.col[j] & puz.matrix[matr_i][matr_j];
			}
		}
	}

	return puz;
}

int CheckSudoku(Puzzles puz)
{
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			if (puz.sudoku[i][j] == 0)
			{
				int matr_i = i / 3;
				int matr_j = j / 3;
				if ((puz.row[i] & puz.col[j] & puz.matrix[matr_i][matr_j]) == 0)
				{
					return FALSE;
				}
			}
		}
	}
	return TRUE;
}

int InBit(int x)
{
	for (int i = 1; i < 10; i++)
	{
		if (x == bit[i])
			return i;
	}
	return FALSE;
}

void Maintain(Puzzles &puz, int i, int j, int num)
{
	int matr_i = i / 3;
	int matr_j = j / 3;
	puz.sudoku[i][j] = num;
	puz.map[i][j] = 0;
	puz.row[i] -= bit[num];
	puz.col[j] -= bit[num];
	puz.matrix[matr_i][matr_j] -= bit[num];
	puz.empty--;
}

void Set_Num(Puzzles &puz)
{
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			if (puz.sudoku[i][j] > 0)continue;
			int num = InBit(puz.map[i][j]);
			if (num != FALSE) {
				Maintain(puz, i, j, num);
				//return;
			}
		}
	}
}

void BackTrace(int t, Puzzles &puz) {
	if (t > 80)
	{
		result = puz;
		return;
	}
	else {
		int row = t / 9;
		int col = t % 9;
		if (puz.sudoku[row][col] != 0)
		{
			BackTrace(t + 1, puz);
			return;
		}
		int able_set[9];
		int cnt = 0;
		//vector<int> able_set;
		int matr_i = row / 3;
		int matr_j = col / 3;
		int able = puz.row[row] & puz.col[col] & puz.matrix[matr_i][matr_j];
		for (int i = 0; i < 9; i++)
		{
			if ((able % 2) == 1)
				able_set[cnt++] = i + 1;
				//able_set.push_back(i + 1);
			able /= 2;
		}
		for (int i = 0; i < cnt; i++)
		{
			Puzzles temp = puz;
			Maintain(temp, row, col, able_set[i]);
			if (CheckSudoku(temp) == TRUE)
				//if ((1 + (int)(rand() % 10)) > 8)Set_Num(temp);
				BackTrace(t + 1, temp);
		}
	}
}


void Solve(Puzzles &puz)
{
	if (CheckSudoku(puz) == FALSE) {
		return;
	}
	if (puz.empty == 0)return;
	Set_Num(puz);
	BackTrace(0, puz);
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			result_set[cnt1++] = result.sudoku[i][j] + '0';
			if (j < 8)result_set[cnt1++] = ' ';
			else if (j == 8)result_set[cnt1++] = '\n';
		}
	}

}

void Write2File1()
{
	FILE *fp;
	fp = fopen("sudoku.txt", "w");
	if (fp == NULL)
	{
		cout << "Open File sudoku.txt Failed!" << endl;
		return;
	}
	fputs(result_set, fp);
	fclose(fp);

}

void Solve_Sudoku(char addr[])
{
	FILE *fr = fopen(addr, "r");
	if (fr == NULL)
	{
		cout << "Open File SudokuQuestions.txt Failed!" << endl;
		return;
	}

	Puzzles puz;

	char ch = '1';
	do {
		if (ch == '\n')result_set[cnt1++] = '\n';
		puz = ReadPuzzle(fr);
		Solve(puz);

	} while ((ch = fgetc(fr)) != EOF);
	Write2File1();
}
