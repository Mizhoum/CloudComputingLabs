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
ofstream out("out.txt");
     ofstream out1( "obj.txt" );
const int lim_puzzle = 1024;
int thread_num = 0;

int **buf;
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
    
//    int end_of_file = 0;
//    thread_num = thread::hardware_concurrency();
//    thread th[thread_num];
    
  //  string prefix = "./data/";
    string name;
       int K=0;
     int line_count = 0;
        char ch;
    while (getline(cin, name)) {
    char *tmp_name = new char[40];
   // name = prefix + name;
    name = name;
    strcpy(tmp_name, name.c_str());
     ifstream in(tmp_name); //源文件读
      while( !in.eof() ) //文件未结束，循环
    {
        char str[1024];
        in.getline(str,sizeof(str),'\n'); //读一行
       // cout << str << endl; //输出到屏幕
       if(!in.eof())
        out1<< str <<endl; //输出到文件
         else
        out1 << str;
    }

    fp = fopen(tmp_name, "r");
     if (fp == NULL) {
      printf("File cannot open!not good!\n");
      exit(0);
    }
}
  fp1 = fopen("obj.txt", "r");
 int64_t start = now();
    int end_of_file = 0;
    thread_num = thread::hardware_concurrency();
    thread th[thread_num];
    	while(end_of_file!=1)
    	{

          int line_count = 0;
          do{
             char* ret_gets = fgets(puzzle, sizeof puzzle, fp1);
             if(ret_gets == NULL)
             {
                end_of_file = 1;
                break;
             }
             if (strlen(puzzle) >= N)
             {
                trans(puzzle, buf[line_count]);
                line_count++;
		++total;
             }
          }while(line_count < lim_puzzle-1);

          for(int i = 0; i < line_count; ++i)
          {
              is_solved[i] = solve_sudoku_dancing_links(buf[i]);
          }
          int step = (line_count+thread_num-1)/thread_num;
	  int curr = 0;
          for(int i = 0; i < thread_num; ++i,curr+=step)
          {
              th[i] = thread(thread_work, curr, ((curr+step>=line_count)?(line_count-curr):step));
          }
          for(int i = 0; i < thread_num; ++i) 
	  {
		th[i].join();
          }
          for(int i = 0; i < line_count; ++i)
          {
              if(is_solved[i])
              {
                 for(int j = 0; j < N; j++) out << buf[i][j];
                 out<<'\n';
		++total_solved;
              }
              else out<<"No result.\n";
          }
    }


    int64_t end = now();
    double sec = (end-start)/1000000.0;
    printf("%f sec %f ms each %d\n", sec, sec/total, total_solved);
    return 0;
}
