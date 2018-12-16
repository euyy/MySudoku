#include"generator.h"



int permutation_num[8] = { 2,3,4,5,6,7,8,9 };
int first_section[2][3] = { 0,3,6,0,6,3 };
int second_section[6][3] = { {1,4,7},{1,7,4},{4,1,7},{4,7,1},{7,1,4},{7,4,1} };
int third_section[6][3] = { {2,5,8},{2,8,5},{5,2,8},{5,8,2},{8,2,5},{8,5,2} };

vector<Sudoku> finals_set;



int* Combination(int a[3], int b[3], int c[3])
{
	int d[9];
	for (int i = 0; i < 3; i++)
	{
		d[i] = a[i];
		d[i + 3] = b[i];
		d[i + 6] = c[i];
	}
	return d;
}
		
Sudoku Transfor(int *seq, int *remv)
{
	Sudoku result;
	string tempstr;
	char tempch;
	
	for (int i = 0; i < 9; i++)
	{
		tempstr = "";
		for (int j = 0; j < 9; j++)
		{
			tempch = seq[(j + remv[i]) % 9] + '0';
			tempstr += tempch;
			if (j < 8)tempstr += ' ';
		}
		result.row[i] = tempstr;
	}
	return result;
}

void WriteDown()
{
	fstream f("sudoku.txt", ios::out);
	int finals_num = finals_set.size();
	for (int cnt=0;cnt<finals_num;cnt++)
	{
		for (int col_num = 0; col_num < 9; col_num++)
		{
			f << finals_set[cnt].row[col_num] << endl;
		}
		if (cnt < finals_num - 1)f << endl;
	}
	f.close();
	

}


void Generate_Sudoku(int finals_num)
{
	int sequence[9];
	int *remove1;
	int *remove2;
	int rest = finals_num;
	Sudoku finals1;
	Sudoku finals2;
	sequence[0] = 1;
	
	
	//int times = (finals_num + 143) / 144;//每次生成144个数独，需要进行  times次生成
	while (1) {
		

		//生成一个序列
		for (int i = 0; i < 8; i++)
		{
			sequence[i + 1] = permutation_num[i];
		}

		/*cout << "rest finals number is : " << rest << endl;
		cout<<"sequence is:";
		for (int x = 0; x < 9; x++)
			cout << sequence[x] << ' ';
		cout << endl<<endl;*/

		int flag = 0;

		//平移生成两个正确的数独终局
		for (int i = 0; i < 2; i++)
		{
			for (int j = 0; j < 6; j++)
			{
				for (int k = 0; k < 6; k++)
				{
					remove1 = Combination(first_section[i], second_section[j], third_section[k]);
					remove2 = Combination(first_section[i], third_section[k], second_section[j]);
					finals1 = Transfor(sequence, remove1);
					//finals1.Print();cout << endl;
					finals_set.push_back(finals1);
					rest--;
					if (!rest) {
						flag = 1;
						break;
						//return;
					}
					finals2 = Transfor(sequence, remove2);
					//finals2.Print();
					finals_set.push_back(finals2);
					rest--;
					if (!rest) {
						flag = 1;
						break;
						//return;
					}
				}
				if (flag == 1)break;
			}
			if (flag == 1)break;
		}
		if (flag == 1)break;
		next_permutation(sequence, sequence + 9);
	}
	WriteDown();
}
