/* 朱泽凯 2051995 济美 */

#ifndef _90_02_b4_hehepig
#define _90_02_b4_hehepig

//包含两种朴素的打印方式（*点阵、hdc点阵）
void print_basic(const char *text, int is_fan, int num_row, int num_col,  int bgcolor,  int fgcolor, int size, bool use_hdc);

//钛金眼模式
void print_colorful(const char *text, int is_fan, int num_row, int num_col, int size);

//向内扩散模式
void print_zoom(const char *text, int is_fan, int num_row, int num_col, int bgcolor, int fgcolor, int size);

//溶解模式
void print_dissolve(const char *text, int is_fan, int num_row, int num_col, int bgcolor, int fgcolor, int size);

//无限长循环播放模式
void print_inf(const char *text, int is_fan,int num_row,  int num_col, int bgcolor, int fgcolor, int size);






#endif

