/* 2051995 ���� ���� */
#include <iostream>
#include <iomanip>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <time.h>
/* �������Ҫ���˴��������ͷ�ļ� */

using namespace std;

/* �����峣����/�궨�壬����������ȫ�ֱ��� */
const unsigned long long BJ_ID = 1ull << 52;
const unsigned long long RJ_ID = 1ull << 53;
const char number[13] = { '3', '4', '5', '6', '7', '8', '9', 'T', 'J', 'Q', 'K', 'A', '2' };
#if defined(__linux) || defined(__linux__)
const char pattern[4] = { 'D', 'H', 'C', 'S' };
#else
const char pattern[4] = { 4, 3, 5, 6 };
#endif
#define ID(no,pat) (1ull<<(((no)<<2)|(pat)))

/* ��������Լ���Ҫ�ĺ��� */

int Get_int(const char *prompt, const int Min, const int Max)
{
    int ret;
    do {
        puts("");
        puts(prompt);
        if (!cin.good()) {
            cin.clear();
            cin.ignore(1024, '\n');
        }
        cin >> ret;
    } while (!cin.good() || !(ret >= Min && ret <= Max));
    return ret;
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ���ӡĳ����ҵ�������Ϣ������ǵ���������ӱ��
  ���������
  �� �� ֵ��
  ˵    ����
 ***************************************************************************/
int print(const char *prompt, const bool landlord, const unsigned long long player)
{
    /* ֻ�����岻���������������͵ļ򵥱��������ܶ�������������ṹ�塢string�� */
    cout << prompt;
    int i, j;
    for (i = 0; i < 13; i++)
        for (j = 0; j < 4; j++)
            if (player & ID(i, j))
                cout << pattern[j] << number[i] << ' ';
    if (player & BJ_ID)
        cout << "BJ" << ' ';
    if (player & RJ_ID)
        cout << "RJ" << ' ';

    if (landlord)
        cout << "(����)";
    cout << endl;
    return 0;
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�����
  ���������
  �� �� ֵ��
  ˵    ����
 ***************************************************************************/
int deal(unsigned long long *player)
{
    /* ֻ�����岻����ʮ���������͵ļ򵥱��������ܶ�������������ṹ�塢string�� */
    
    srand((unsigned int)time(0));

    unsigned long long status_all = 0;  //��¼��Щ�Ʒ�����
    unsigned long long dizhu;   //���ŵ�����
    int lun;  //�ڼ���
    int i, j;  //��ʱ����
    int dizhuID=-1;    //˭�ǵ���;

    for (lun = 1; lun <= 17; lun++) {
        for (i = 0; i < 3; i++) {
            do {
                j = rand() % 54;
            } while ((1ull << j) & status_all);
            status_all |= player[i] |= (1ull << j);
        }
        cout << "��" << lun << "�ֽ�����" << endl;
        print("�׵��ƣ�", 0, player[0]);
        print("�ҵ��ƣ�", 0, player[1]);
        print("�����ƣ�", 0, player[2]);
    }

    dizhu = ((1ull << 54) - 1) ^ status_all;
    dizhuID = Get_int("��ѡ��һ������[0-2]��", 0, 2);
    player[dizhuID] |= dizhu;

    //print("�����ƣ�", 0, dizhu);

    return dizhuID; //�˴��޸�Ϊѡ���ĵ���(0-2)
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ����main��������׼�޸�
 ***************************************************************************/
int main()
{
    /*
    for (int i = 0; i < 13; i++)
        for (int j = 0; j < 4; j++)
            printf("[%d %d]%lld\n", i, j, ID(i, j));
    printf("%lld\n%lld\n", BJ_ID, RJ_ID);
    printf("%lld\n", 1ll << 53);
    */

    unsigned long long player[3] = { 0 }; //���������ҵķ�����Ϣ
    int landlord; //����0-2��ʾ�ĸ�����ǵ���

    cout << "���س�����ʼ����";
    while (getchar() != '\n')
        ;

    landlord = deal(player);
    print("�׵��ƣ�", (landlord == 0), player[0]);
    print("�ҵ��ƣ�", (landlord == 1), player[1]);
    print("�����ƣ�", (landlord == 2), player[2]);

    return 0;
}
