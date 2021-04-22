/* 2051995 ���� ���� */
#include <iostream>
using namespace std;

const int ROW = 2;
const int COL = 3;

#define OPERATOR_IS_MEMBER_FUNCTION		1	//������������ó�Ա������ʽʵ�֣����ֵΪ0����ʾ����Ԫ��ʽʵ�֣�

class matrix {
private:
    int value[ROW][COL];
    //����֮�ⲻ������������ݳ�Ա
public:
    matrix(const int(*p)[COL]);	//���캯�����������ʼ������
    matrix(); //ȫ�����

#if OPERATOR_IS_MEMBER_FUNCTION
    //����+�ĳ�Ա����ʵ�ַ�ʽ������
    matrix operator+(const matrix &B);
#else
    //����+����Ԫ����ʵ�ַ�ʽ������
    friend matrix operator+(const matrix &A, const matrix &B);
#endif

    /* ----������Ҫ���������Ա�����Ķ������Ԫ��������������������---- */

    int(&operator[](int x))[COL];

    //������������
    friend istream &operator>> (istream &in, matrix &M);
    friend ostream &operator<< (ostream &out, const matrix &M);

};

/* ---- ������Ա����������ʵ��/��Ԫ������ʵ�֣�����+��ʵ���г�Ա����Ԫ������ʽ��Ҫ����������Ԥ������ ---- */

matrix::matrix(const int(*p)[COL])
{
    for (int i = 0; i < ROW; i++)
        for (int j = 0; j < COL; j++)
            this->value[i][j] = p[i][j];
}

matrix::matrix()
{
    for (int i = 0; i < ROW; i++)
        for (int j = 0; j < COL; j++)
            this->value[i][j] = 0;
}

istream &operator>> (istream &in, matrix &M)
{
    for (int i = 0; i < ROW; i++)
        for (int j = 0; j < COL; j++)
            in >> M.value[i][j];
    return in;
}

ostream &operator<< (ostream &out, const matrix &M)
{
    for (int i = 0; i < ROW; i++) {
        for (int j = 0, mj = COL - 1; j < mj; j++)
            out << M.value[i][j] << ' ';
        out << M.value[i][COL - 1] << endl;
    }
    return out;
}

int(&matrix::operator[](int x))[COL] {
    return (int(&)[COL]) (value[x]);
}

#if OPERATOR_IS_MEMBER_FUNCTION
//����+�ĳ�Ա������ʽ������ʵ��
matrix matrix::operator+(const matrix &B)
{
    matrix C;
    for (int i = 0; i < ROW; i++)
        for (int j = 0; j < COL; j++)
            C[i][j] = this->value[i][j] + B.value[i][j];
    return C;
}

#else
    //����+����Ԫ������ʽ������ʵ��
    matrix operator+(const matrix &A, const matrix &B)
{
    matrix C;
    for (int i = 0; i < ROW; i++)
        for (int j = 0; j < COL; j++)
            C[i][j] = A.value[i][j] + B.value[i][j];
    return C;
}
#endif


/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ����main������׼��
***************************************************************************/
int main()
{
    int a[ROW][COL] = { { 1,2,3 },{ 4,5,6 } };
    int b[ROW][COL] = { { 2,4,6 },{ 3,6,9 } };
    matrix m1(a), m2(b), m3; //m3�ĳ�ֵΪȫ0

    /* ����Ϊ��ֵ���� */
    cout << "��ʼ����m1��ֵ" << endl;
    cout << m1 << endl;
    cout << "��ʼ����m2��ֵ" << endl;
    cout << m2 << endl;

    m3 = m1 + m2;
    cout << "��Ӻ����m3��ֵ" << endl;
    cout << m3 << endl;

    cout << "------------------" << endl;

    /* ����Ϊ����������� */
    cout << "�������ʼ����m1" << endl;
    cin >> m1;
    cout << "�������ʼ����m2" << endl;
    cin >> m2;

    cout << "��ʼ����m1��ֵ" << endl;
    cout << m1 << endl;
    cout << "��ʼ����m2��ֵ" << endl;
    cout << m2 << endl;
    cout << "��Ӻ����m3��ֵ" << endl;
    cout << (m3 = m1 + m2) << endl;

    return 0;
}
