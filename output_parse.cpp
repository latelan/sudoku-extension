#include <stdio.h>
#include <stdlib.h>
#include "common.h"


void output_parse(const char *output_file, const SUDOKU_RES (&result)[101], const int& result_cnt)
{
	FILE *out;
	out = fopen(output_file,"w");
	if(!out){
		printf("write failed.\n");
		return;
	}

	for(int k=0; k<result_cnt; k++){
		fprintf(out, "%d\n", result[k].case_num);

		if(result[k].score == 0){
			fprintf(out,"impossible\n");
		} else {
			fprintf(out, "%d\n", result[k].score);
			for(int i=0; i<9; i++){
				for(int j=0; j<9; j++){
					fprintf(out,"%c",result[k].s[i*9+j]);
				}
				fprintf(out,"\n");
			}
		}
		fprintf(out,"---\n");
	}

	fclose(out);
}

void output_to_cmd(const SUDOKU_RES (&result)[101], const int &result_cnt)
{
	for(int k=0; k<result_cnt; k++) {
		printf("%d\n",result[k].case_num);

		if(result[k].score == 0){
			printf("impossible\n");
		} else {
			printf("%d\n", result[k].score);
			for(int i=0; i<9; i++) {
				for(int j=0; j<9; j++) {
					printf("%c",result[k].s[i*9+j]);
				}   
				printf("\n");
			}   
		}
		printf("---\n");
	}
}
