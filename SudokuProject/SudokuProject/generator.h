#pragma once
#include<algorithm>
#include"common.h"
#include <stdio.h>
using namespace std;
extern int cnt;
extern int rest;
extern int remv[144][9];
extern char finals_buffer[163000000];

void Write2File();
void Secq2Final(int seq[]);
void Generate_Sudoku(int finals_num);