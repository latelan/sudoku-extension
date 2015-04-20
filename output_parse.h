#ifndef _OUTPUT_PARESE_H_
#define _OUTPUT_PARESE_H_
/* output_parse.h */
#include "common.h"

void output_parse(const char *output_file, const SUDOKU_RES (&result)[101],const int& result_cnt);
void output_to_cmd(const SUDOKU_RES (&result)[101], const int &case_cnt);

#endif /* _OUTPUT_PARSE_H_ */
