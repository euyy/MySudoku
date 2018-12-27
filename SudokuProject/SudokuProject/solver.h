#pragma once
#include"common.h"
#include<iostream>
#include<vector>
#include<stdlib.h> 
#pragma warning(disable : 4996)
using namespace std;

struct Puzzles {
	int sudoku[9][9];//数独谜题
	int map[9][9];//每个格子有哪些数可以填写
	int row[9];//每一行有哪些数可以填写
	int col[9];//每一列有哪些数可以填写
	int matrix[3][3];//每一个3*3方阵有哪些数可以填写
	int empty;//还有几个未填写的格子
};


extern char result_set[163000000];
extern int cnt1;
extern Puzzles result;
extern int bit[10];

void Init(Puzzles &puz);
void Generate_Map(Puzzles &puz);
Puzzles ReadPuzzle(FILE* fr);
int CheckSudoku(Puzzles puz);
int InBit(int x);
void Maintain(Puzzles &puz, int i, int j, int num);
void Set_Num(Puzzles &puz);
void BackTrace(int t, Puzzles &puz);
void Solve(Puzzles &puz);
void Write2File1();

void Solve_Sudoku(char addr[]);