#ifndef _DRIVER_H_
#define _DRIVER_H_
/* driver.h */

int driver(const char *input_file, const char *output_file);
void input_parse();
void input_parse_opt();
int get_score(const char (&s)[82]);

#endif /* _DRIVER_H_ */
