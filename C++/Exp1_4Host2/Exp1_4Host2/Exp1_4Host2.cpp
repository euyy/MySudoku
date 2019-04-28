#include"myRecv.h"
#include"mySend.h"
#include<thread>
using namespace std;


int main() {
	thread t1(myReceive);
	thread t2(mySend);
	t1.join();
	t2.join();
	system("pause");
	return 0;
}