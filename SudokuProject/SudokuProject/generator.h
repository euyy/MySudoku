#pragma once
#include<algorithm>
#include<vector>
#include<fstream>
#include"common.h"
#include <stdio.h>
using namespace std;

struct Sudoku {
	string row[9];
	void Print() {
		for (int i = 0; i < 9; i++)
			cout << row[i] << endl;
	}
};

void Generate_Sudoku(int finals_num);

