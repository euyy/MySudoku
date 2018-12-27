#pragma once
#include"common.h"
#include<iostream>
#include<vector>
#include<stdlib.h> 
#pragma warning(disable : 4996)
using namespace std;

struct Puzzles {
	int sudoku[9][9];//��������
	int map[9][9];//ÿ����������Щ��������д
	int row[9];//ÿһ������Щ��������д
	int col[9];//ÿһ������Щ��������д
	int matrix[3][3];//ÿһ��3*3��������Щ��������д
	int empty;//���м���δ��д�ĸ���
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