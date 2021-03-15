/* 朱泽凯 济美 2051995 */
#define once

#define Maxlen (40)
#define OPR_OPEN 0
#define OPR_FLAG 1
#define OPR_TIME 2
#define OPR_EXIT 3
#define OPR_CHEK 4



const int COL_BG[3] = { COLOR_YELLOW,COLOR_PINK,COLOR_WHITE };
const int COL_FG[10] = { 0,1,2,5,3,12,9,10,11,13 };


struct GMDATA
{
	int mode;
	int level;
	int Xlen, Ylen;
	int bombnum;
	int map[Maxlen][Maxlen];	//下标从 1 开始
	int tag[Maxlen][Maxlen];	//0:未开封  1:插旗  2:显示 
	double start_time;
	int flagnum;
};
//(double)(finish - start) / CLOCKS_PER_SEC

struct OPERATION
{
	int mode=0;	//0:打开  1:插/收旗  2:运行时间  3:退出
	int X=0, Y=0;
};

const int fx[9][2] = { {-1,-1},{-1,0},{-1,1},{0,-1},{0,1},{1,-1},{1,0},{1,1},{0,0} };

/* menu.cpp */
GMDATA MENU();
void init_gmdata(GMDATA& ret);

/* tools.cpp */
int my_getch(const int L, const int R);

/* base.cpp */
int PLAY(GMDATA gmdata);

/* graph.cpp */
void graph_show(const GMDATA& D);
void graph_draw_frame(const GMDATA& D);
void graph_draw_num(const int X, const int Y, const char C = ' ', const int BGCOL = COL_BG[0], const int FGCOL = 7);
void get_graph_opr(const GMDATA& D, OPERATION &opr);
void graph_show_bombs(const GMDATA& D);