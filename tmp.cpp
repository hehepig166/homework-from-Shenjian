//�ȴ�Windows��������������ٹ۲���������

#include <iostream>
#include <cstdlib>
using namespace std;

int main()
{
    char *p;// , *q;

    p = (char *)malloc(100 * 1024 * 1024 * sizeof(char)); //100MB���˴�Ҫ��֤�ɹ�
    if (p == NULL) {
        cout << "����ռ�ʧ�ܣ����������ֵ������" << endl;
        return -1;
        }
    cout << "������ɣ���۲��ڴ�ռ��" <<endl;
    getchar(); //��ͣ�����ͷ��ڴ�

    p = (char *)realloc(p, 2048U * 1024 * 1024 * sizeof(char)); //2GB���˴�Ҫ��֤ʧ�ܣ������ʧ�ܣ���������ֵ
    if (p==NULL) //�������ʾʧ�ܣ���2048��������
        cout << "reallocʧ�ܣ���۲��ڴ�ռ��" <<endl;
    getchar(); //��ͣ�����˳�����

    free(p);
    return 0;
}

