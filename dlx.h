#ifndef SODoKU_H_
#define SUDOKU_H_

void init();
void Remove(const int& c);
void Resume(const int& c);
inline void addcol(const int& r,const int& c);
inline void addrow(const int& i,const int& j,const int& k);
int need_addrow(const int& i,const int& j,const int& k);
void addrow_opt(const int& i,const int& j,const int& k,const int& is_known);
void build();
int get_score(const char (&s)[82]);
bool Dance_opt(const int& k);
inline void get_max_score();
bool Dance(const int& k);
bool worker(const char (&map_origin)[82],char *ans_str,int &score);

#endif

