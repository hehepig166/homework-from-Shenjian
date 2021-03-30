//先打开Windows的任务管理器，再观察程序的运行

#include <iostream>
#include <cstdlib>
using namespace std;

int main()
{
    char *p;// , *q;

    p = (char *)malloc(100 * 1024 * 1024 * sizeof(char)); //100MB，此处要保证成功
    if (p == NULL) {
        cout << "申请空间失败，请减少申请值后重试" << endl;
        return -1;
        }
    cout << "申请完成，请观察内存占用" <<endl;
    getchar(); //暂停，不释放内存

    p = (char *)realloc(p, 2048U * 1024 * 1024 * sizeof(char)); //2GB，此处要保证失败，如果不失败，继续增大值
    if (p==NULL) //如果不提示失败，则2048继续增大
        cout << "realloc失败，请观察内存占用" <<endl;
    getchar(); //暂停，不退出程序

    free(p);
    return 0;
}

