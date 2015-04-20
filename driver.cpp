/*
 * driver
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "common.h"
#include "driver.h"
#include "dlx.h"
#include "output_parse.h"

#define N 			9
#define MAX_CASE 	101       // 一次输入最大case数

const int KNOWN		= 1;	  // 表示已知格子
const int UNKNOWN	= 0;

static char map[N*N+1];		  // 待解数独
SUDOKU_RES result[MAX_CASE];  // 每个case的结果
int result_cnt;				  // 结果个数
int case_num;				  // case编号

int driver(const char *input_file, const char *output_file)
{
	char end_str[4];
	int case_num;
	FILE *fp = fopen(input_file,"r");
	if(!fp) {
		printf("open failed.\n");
		return 1;
	}

	result_cnt = 0;
	while(!feof(fp)) {
		fscanf(fp, "%d", &case_num);

		int i = 0;
		while(i<9) {
			fscanf(fp, "%s", &map[i*9]);
			i++;
		}

		fscanf(fp, "%s\n", end_str);
		
		char s[82];
		int score = 0;
		int ret = false;
		result[result_cnt].case_num = case_num;

		memset(s,0,sizeof(s));
		ret = worker(map,s,score);
		if(ret == true) {
			result[result_cnt].score = score;
			strcpy(result[result_cnt].s, s);
		} else {
			result[result_cnt].score = 0;
		}

		result_cnt++;
	}

	//output_parse(output_file, result,result_cnt);
	output_to_cmd(result,result_cnt);

	fclose(fp);

	return 0;
}

int main(int argc, char **argv)
{
	if(argc != 2) {
		printf("Usage: %s input.txt output.txt\n", argv[0]);
		return 0;
	}

	driver(argv[1],argv[2]);

	return 0;
}
