/* ���� 2051995 ���� */

#ifndef _90_02_b4_hehepig
#define _90_02_b4_hehepig

//�����������صĴ�ӡ��ʽ��*����hdc����
void print_basic(const char *text, int is_fan, int num_row, int num_col,  int bgcolor,  int fgcolor, int size, bool use_hdc);

//�ѽ���ģʽ
void print_colorful(const char *text, int is_fan, int num_row, int num_col, int size);

//������ɢģʽ
void print_zoom(const char *text, int is_fan, int num_row, int num_col, int bgcolor, int fgcolor, int size);

//�ܽ�ģʽ
void print_dissolve(const char *text, int is_fan, int num_row, int num_col, int bgcolor, int fgcolor, int size);

//���޳�ѭ������ģʽ
void print_inf(const char *text, int is_fan,int num_row,  int num_col, int bgcolor, int fgcolor, int size);






#endif

