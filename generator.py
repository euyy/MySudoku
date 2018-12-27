#coding=utf-8
import sys
import random
finals_num = 0
count = 1
num_set = [1,2,3,4,5,6,7,8,9]
first=[[0,3,6],[0,6,3],[3,0,6],[3,6,0],[6,0,3],[6,3,0]]
second=[[1,4,7],[1,7,4],[4,1,7],[4,7,1],[7,1,4],[7,4,1]]
third=[[2,5,8],[2,8,5],[5,2,8],[5,8,2],[8,2,5],[8,5,2]]
pianyi=[-10,-9,-8,-1,0,1,8,9,10]
centr=[10,13,16,37,40,43,64,67,70]
remvs=[]
questions=[]

def Combination(first,second,third):
    global remvs
    for i in range(6):
        for j in range(6):
            for k in range(6):
                remv=first[i]+second[j]+third[k]
                remvs.append(remv)

def GeneratingRemoveWay():
    Combination(first,second,third)
    Combination(first,third,second)
    Combination(second,first,third)
    Combination(second,third,first)
    Combination(third,first,second)
    Combination(third,second,first)
    
def Question(seq,remv_num):
    global finals_num
    global count
    finals=[]
    for i in range(9):
        for j in range(9):
            num=seq[(j+remvs[remv_num][i])%9]
            finals.append(num)

    for i in range(9):
        temp=random.randint(4,5)
        choose_num=random.sample(pianyi,temp)
        for j in range(temp):
            finals[centr[i]+choose_num[j]]=0
   
    questions.append(finals)
    finals_num = finals_num - 1
    count = count + 1
    if(count%10000==0):
        print(count)


def GeneratingQuestion():
    while(finals_num!=0):
        for i in range(1000):
            random.shuffle(num_set)
            remv_num=random.randint(0,1295)
            Question(num_set,remv_num)
            if(finals_num==0):
                break

def Write2File():
    fw = open('SudokuQuestions.txt','w')
    for i in range(len(questions)):
        for j in range(9):
            for k in range(9):
                fw.write(str(questions[i][j*9+k]))
                if(k<8):
                    fw.write(' ')
                elif(k==8):
                    fw.write('\n')
        if(i<(len(questions)-1)):
            fw.write('\n')


if __name__ == '__main__':
    finals_num = int(input("finals number is:"))
    GeneratingRemoveWay()
    GeneratingQuestion()
    Write2File()
