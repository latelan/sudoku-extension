#ifndef _COMMON_H_
#define _COMMON_H_

// 一个case的解
typedef struct sudoku_res {
	int case_num; // case 编号
	int score;    // 得分
	char s[82];   // 矩阵
}SUDOKU_RES;


#endif 
