#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <thread>
#include <pthread.h>
#include <iostream>
#include <fstream>
#include "sudoku.h"

using namespace std;
const int lim_puzzle = 2000000;
int thread_num = 0;

int **buf= new int* [lim_puzzle];
int **buf1= new int* [lim_puzzle];
bool *is_solved= new bool [lim_puzzle];
char puzzle[2000000];
int line[2000000];

int64_t now()
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec * 1000000 + tv.tv_usec;
}

void thread_work(const int sta, const int line_num)
{
    for(int i = 0; i < line_num; ++i)
    {
        is_solved[sta+i] = solve_sudoku_dancing_links(buf[sta+i]);
    }
}

int main(int argc, char* argv[])
{
    FILE *fp;
    FILE *fp1;
    int total_solved = 0;
    int total = 0;
    for(int i = 0; i < lim_puzzle; ++i) buf[i] = new int [N];
    for(int i = 0; i < lim_puzzle; ++i) buf1[i] = new int [N];
    int end_of_file = 0;
    thread_num = thread::hardware_concurrency();
    thread th[thread_num];
    string name;
    int K=0;
    int T=0;
    char ch;
    line[0]=0;

    int kkk=0;
    string name_1[1024];
    while (getline(cin, name)){
    	name_1[kkk]=name;
    	kkk++;
	}

//    int64_t start = now();

    int kk=0;
    while (kk<kkk) {
    	name=name_1[kk];
    	int line_count = 0;
    	line[K]==line_count;
    	K++;
    	char *tmp_name = new char[40];
    	name = name;
    	strcpy(tmp_name, name.c_str());
    	fp = fopen(tmp_name, "r");
     	if (fp == NULL) {
      		printf("File cannot open!not good!\n");
    	}
    	end_of_file = 0;
    	while(end_of_file!=1)
    	{
       		int line_count = 0;
        	do{
        		char* ret_gets = fgets(puzzle, sizeof puzzle, fp);
        	    if(ret_gets == NULL)
        	    {
        	        end_of_file = 1;
        	        break;
        	    }
        	    if (strlen(puzzle) >= N)
        	    {
        	        trans(puzzle, buf[line[K]]);
        	        line[K]++;
					++total;
        	    }
        	}while(line[K] < lim_puzzle-1);
		}
		for(int i = 0; i < line[K]; ++i)
    	{
        	is_solved[i] = solve_sudoku_dancing_links(buf[i]);
    	}
    	int step = (line[K]+thread_num-1)/thread_num;
		int curr = 0;
    	for(int i = 0; i < thread_num; ++i,curr+=step)
    	{
        	th[i] = thread(thread_work, curr, ((curr+step>=line[K])?(line[K]-curr):step));
    	}
    	for(int i = 0; i < thread_num; ++i)
		{
			th[i].join();
    	}

		T=T+line[K];
    	for(int i = 0; i < line[K]; ++i)
    	{
        	if(is_solved[i])
        	{
            	for(int j = 0; j < N; j++)
				{
				buf1[i+T-line[K]][j]=buf[i][j];
				}
				++total_solved;
        	}
            else cout<<"No result.\n";
    	}
    kk++;
	}

    int sum=0;
    for(int l=0;l<=K;l++)
    {
        sum=sum+line[l];
	}
	int num=0;
	for(int d=0;d<sum;d++)
	{
		num++;
		for(int j = 0; j < N; j++)
		{
		  	cout << buf1[d][j];
		}
		cout<<endl;
	}
//    int64_t end = now();
//    double sec = (end-start)/1000000.0;
//   printf("%f sec %f ms each %d\n", sec, 1000.0*sec/total, total_solved);
    return 0;
}
