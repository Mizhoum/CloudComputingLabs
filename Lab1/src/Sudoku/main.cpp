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
//ofstream out("out.txt");
     ofstream out1( "obj.txt" );
const int lim_puzzle = 1024;
int thread_num = 0;

int **buf;
    //      int **buf1;
                    int buf1[1024][1024] ;
bool *is_solved;

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
    
   // FILE* fp = fopen(argv[1], "r");
    FILE *fp;
    FILE *fp1;
    char puzzle[128];
    int total_solved = 0;
    int total = 0;
    buf = new int* [lim_puzzle];
    is_solved = new bool [lim_puzzle];
    for(int i = 0; i < lim_puzzle; ++i) buf[i] = new int [N];
     int64_t start = now();
    int end_of_file = 0;
    thread_num = thread::hardware_concurrency();
    thread th[thread_num];
    //char name[10000];
    string name;
       int K=0;
        char ch;
        int line[1024]; 
        line[0]=0;
    while (getline(cin, name)) {
    	     int line_count = 0;
    	line[K]==line_count;
    	K++;
    char *tmp_name = new char[40];
   // name = prefix + name;
    name = name;
    strcpy(tmp_name, name.c_str());
    fp = fopen(tmp_name, "r");
     if (fp == NULL) {
      printf("File cannot open!not good!\n");
     // exit(0);
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
//cout<<"file"<<K<<":"<<line[K]<<endl;
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


          for(int i = 0; i < line[K]; ++i)
          {
              if(is_solved[i])
              {
              //	fp1 = fopen(tmp_name, "w");
             // 	ofstream out(tmp_name);
                 for(int j = 0; j < N; j++) 
				 {
			//	 out << buf[i][j];
			buf1[i+line[K-1]][j]=buf[i][j];
			//	 cout << buf[i][j];
				 
			}
              //  cout<<'\n';
		++total_solved;
              }
              else cout<<"No result.\n";
          }
          } 
          int sum=0;
          for(int l=0;l<=K;l++)
          {
          	sum=sum+line[l];
		  }
//		  cout<<"sum"<<sum<<endl;
		  for(int d=0;d<sum;d++)
		  {
		  	 for(int j = 0; j < N; j++) 
		  	 {
		  	 	cout << buf1[d][j];
			   }
			   cout<<endl;
		  }
    int64_t end = now();
    double sec = (end-start)/1000000.0;
    printf("%f sec %f ms each %d\n", sec, sec/total, total_solved);
    return 0;
}
