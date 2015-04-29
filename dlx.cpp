/*
 * 解数独主要算法
 * referer:  http://blog.csdn.net/ruptins/article/details/12978783
 * created by abel, 2015-04-13 
 */

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <algorithm>
#include "dlx.h"

using namespace std;

#define N 5000
#define M 1100
const int delta[] 	= {1,82,163,244};
const int gird[]	= {80,79,71,70,78,62,69,61,60};
const int nC		= 9*9*4;
const int KNOWN		= 1;
const int UNKNOWN	= 0;
const int head		= 0;

int L[N*4],R[N*4],U[N*4],D[N*4];
int C[N*4]; 	// 记录节点所在行
int X[N*4]; 	// 记录节点所在列
int S[M]; 		// 列链中结点数
int H[N];   	// 行指针头
int Q[N];   	// 输入结点值得队列，Q[i]=n表示第i行是数字n
char map[82];   // 保存输入矩阵
int mk[25];		// 标记相同字母访问情况
int nid;		// 节点ID
int cnt;		
int row;		// 行索引

struct Ans {
	int r;
	int c;
	int k;
}ans[N*4];		// 解集合

int sk[N]; 		// DFS所用栈
int depth = 0; 	// 限制搜索深度
char s[82];		// 保存解
int max_score;	// 最大数独得分
int a_k[9];		// 可供选择的数字

// for test
int num = 0;
int times;      // Dance调用次数

/**
 * 初始化DL
 */
void init(){
	for(int i=0;i<=nC;i++){
		R[i]=i+1;L[i+1]=i;
		U[i]=D[i]=i;
		S[i]=0;
	}
	R[nC] = 0;
	nid = nC;
}
void Remove(const int& c){
	L[R[c]]=L[c];
	R[L[c]]=R[c];
	for (int i=D[c];i!=c;i=D[i]){
		for (int j=R[i];j!=i;j=R[j]){
			U[D[j]]=U[j];
			D[U[j]]=D[j];
			S[C[j]]--;
		}
	}
}
void Resume(const int& c){
	L[R[c]]=c;
	R[L[c]]=c;
	for (int i=U[c];i!=c;i=U[i]){
		for (int j=L[i];j!=i;j=L[j]){
			U[D[j]]=j;
			D[U[j]]=j;
			S[C[j]]++;
		}
	}
}
/**
 * 在行r列c处添加一个结点
 * @param int r - 行号
 *        int c - 列号
 */
inline void addcol(const int& r,const int& c){

	nid++;
	//设置UD
	D[nid]=D[c];U[nid]=c;
	U[D[c]]=nid;D[c]=nid;

	//设置LR，注意区分是否为该行首个元素
	if (H[r]<0) H[r]=L[nid]=R[nid]=nid;
	else {
		L[nid]=H[r];R[nid]=R[H[r]];
		L[R[H[r]]]=nid;R[H[r]]=nid;
	}

	S[c]++;		//该列节点数+1
	X[nid]=r;  
	C[nid]=c;  //记录该节点所在行和列
}

inline void addrow(const int& i, const int& j, const int& k){
	row++;

	H[row] = -1;
	ans[row].r = i;
	ans[row].c = j;
	ans[row].k = k+1;

	addcol(row, i*9+k+delta[0]);
	addcol(row, j*9+k+delta[1]);
	addcol(row, (i/3*3+j/3)*9+k+delta[2]);
	addcol(row, i*9+j+delta[3]);
}
/**
 * 检查是否需要添加该行,和四个约束条件之一冲突则不需添加
 * @param i,j - 行和列
 * 		  k	  - 值
 * @return 1表示需要添加，0不需要
 */
int need_addrow(const int& i,const int& j,const int& k){
	if(S[i*9+k+delta[0]] == -1
			|| S[j*9+k+delta[1]] == -1
			|| S[(i/3*3+j/3)*9+k+delta[2]] == -1
			|| S[i*9+j+delta[3]] == -1){
		return 0;
	}

	return 1;
}
/**
 * 优化的addrow,添加已知数字时，将该列节点数置-1，其他节点就不要添加了
 * @param i,j - 行和列
 * 		  k   - 值
 * 		  is_known - 是否为已知数字
 */
void addrow_opt(const int& i, const int& j, const int& k, const int& is_known=0){
	if(is_known == 0 && need_addrow(i,j,k) == 0){
		return;
	}

	row++;
	H[row] = -1;
	ans[row].r = i;
	ans[row].c = j;
	ans[row].k = k+1;

	addcol(row, i*9+k+delta[0]);
	addcol(row, j*9+k+delta[1]);
	addcol(row, (i/3*3+j/3)*9+k+delta[2]);
	addcol(row, i*9+j+delta[3]);

	if(is_known == 1){
		S[i*9+k+delta[0]] = -1;
		S[j*9+k+delta[1]] = -1;
		S[(i/3*3+j/3)*9+k+delta[2]] = -1;
		S[i*9+j+delta[3]] = -1;
	}
}
/**
 * 将数独矩阵转化为DL
 */
void build(){
	init();

	row=0;
	char c;
	//已知数字
	for(int i=0; i<81; i++){
		c = map[i];
		if(c >= '1' && c <= '9'){
			addrow_opt(i/9, i%9, c-'1', KNOWN);
		}
	}
	//字母e或o
	for(int i=0; i<81; i++){
		c = map[i];
		int start;
		if(c == 'e'){ start = 1; }
		else if(c == 'o'){ start = 0; } 
		else { continue; }

		for(int k=start; k<9; k+=2){
			addrow_opt(i/9, i%9, k, UNKNOWN);
		}
	}

	//相同字母
	int aflag[26];
	memset(aflag,0,sizeof(aflag));
	for(int i=0; i<81; i++){
		c = map[i];
		if(c!='e' && c!='o' && c>='a' && c<='z' && aflag[c-'a']==0){
			int ta_k[9] = {1,1,1,1,1,1,1,1,1};
			int sk_cnt = 0;
			int sk[9];

			aflag[c-'a'] = 1;
			sk[sk_cnt++] = i;
			for(int j=i+1; j<81; j++){ //找到所有字母
				if(c == map[j]){
					sk[sk_cnt++] = j;
				}
			}
			for(int n=0; n<sk_cnt; n++){ //求添加交集
				for(int k=0; k<9; k++){
					if(ta_k[k] == 1 
							&& need_addrow(sk[n]/9,sk[n]%9,k) == 0){
						ta_k[k] = 0;
					}
				}
			}
			for(int k=0; k<9; k++){ //添加
				if(ta_k[k] == 1){
					for(int n=0; n<sk_cnt; n++){
						addrow(sk[n]/9, sk[n]%9, k);
					}
				}
			}
		}
	}
	//未知
	for(int i=0; i<81; i++){
		c = map[i];
		if(c == '0'){
			for(int k=0; k<9; k++){
				addrow_opt(i/9, i%9, k, UNKNOWN);
			}
		}
	}
}

int get_score(const char (&s)[82]) 
{
	int sum = 0;
	for(int i=0; i<9; i++) {
		for(int j=0; j<9; j++) {
			sum += (i+1) * (j+1) * (s[i*9+j]-'0');
		}   
	}   

	return sum;
}
bool Dance_opt(const int& k){

	if(++times > 500000){
		cnt = -1;
		return false;
	}
	if(R[head] == head){
		cnt++;
		get_max_score();
		return true;
	}

	int min = N,c;
	for(int i=R[head]; i!=head; i=R[i]){
		if(min > S[i]){
			min = S[i];
			c = i;
		}
	}

	Remove(c);
	for(int i=D[c]; i!=c; i=D[i]){

		int col=ans[X[i]].k;
		int tx=ans[X[i]].r;
		int ty=ans[X[i]].c;
		int tp=map[tx*9+ty]-'a';
		int tk=mk[tp];
		if (tp!=4&&tp!=14&&tp>=0&&tp<=25){
			if (mk[tp]==-1) mk[tp]=col;
			else if (mk[tp]!=col) continue;
		}
		sk[k] = X[i];
		for (int j=R[i];j!=i;j=R[j]){
			Remove(C[j]);
		}
		if(Dance_opt(k+1)){ 
			return true;
		}
		if (tp!=4&&tp!=14&&tp>=0&&tp<=25&&tk==-1)
			mk[tp]=-1;
		for (int j=L[i];j!=i;j=L[j]){
			Resume(C[j]);
		}
	}
	Resume(c);

	return false;
}
/**
 * 将最优解保存到s和max_score中
 */
inline void get_max_score(){
	char ts[82];
	memset(ts,0,sizeof(ts));
	for(int i=0; i<81; i++){
		ts[ans[sk[i]].r*9 + ans[sk[i]].c] = ans[sk[i]].k+'0';
	}
	//puts(ts);
	int tscore = get_score(ts);
	if(max_score < tscore){
		strcpy(s,ts);
		max_score = tscore;
	}
}
bool Dance(const int& k){
	if(++times > 200000){
		cnt = -1;
		return false;
	}
	if (R[head]== head){
		cnt++;  // 解个数+1
		get_max_score();
		return true;
	}
	//选出结点数最少的一列,进行标记
	int min=N,c;
	for (int i=R[head];i;i=R[i]){
		if (min>S[i]){
			min=S[i];
			c=i;
		}
	}

	Remove(c);
	for(int i=D[c]; i!=c; i=D[i]){

		int col=ans[X[i]].k;
		int tx=ans[X[i]].r;
		int ty=ans[X[i]].c;
		int tp=map[tx*9+ty]-'a';
		int tk=mk[tp];
		if (tp!=4&&tp!=14&&tp>=0&&tp<=25){
			if (mk[tp]==-1) mk[tp]=col;
			else if (mk[tp]!=col) continue;
		}
		sk[k] = X[i];
		for (int j=R[i];j!=i;j=R[j]) Remove(C[j]);
		Dance(k+1);
		if (tp!=4&&tp!=14&&tp>=0&&tp<=25&&tk==-1)
			mk[tp]=-1;
		for (int j=L[i];j!=i;j=L[j]) Resume(C[j]);
	}
	Resume(c);

	return false;
}
/**
 * 计算节点数目
 */
int get_nodes_sum()
{
	int sum = 0;
	for(int i=R[head]; i!=head; i=R[i]){
		sum += S[i];
	}

	return sum;
}

/**
 * 求解接口
 * @param map_orign - 待解数独矩阵
 * 		  ans_str   - 保存解
 * 		  score		- 对应的数独得分
 * @return true表示有解，false表示无解
 */
bool worker(const char (&map_origin)[82], char *ans_str, int &score){
	memset(mk,-1,sizeof(mk));

	strcpy(map,map_origin);
	max_score = -1;
	cnt = 0;

	build();
/*
	times = 0;
	int cols = get_nodes_sum();
	if(cols > 1500){
		Dance_opt(0);
	} else {
		Dance(0);
	}

	if(times >= 200000){ // 大于限定时间，采取另一策略
		times = 0;
		memset(mk,-1,sizeof(mk));
		max_score = -1;
		cnt = 0;

		build();
		Dance_opt(0);
	}
*/
	times = 0;
	Dance(0);
	if(times >= 200000){
		times = 0;
		Dance_opt(0);
	}

	//	Dance_opt(0);
	if(max_score != -1){
		strcpy(ans_str,s);
		score = max_score;
	} else {
		return false;
	}

	return true;
}
