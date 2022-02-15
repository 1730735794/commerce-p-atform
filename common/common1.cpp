#include<iostream>
using namespace std;
int ** Initialize()
{
    int **p = new int *[4];
    for(int i = 0; i < 4; i++)
    {
        p[i] = new int[5];
    }
    for(int i = 0; i < 4; i++)
    {
        for(int j = 0; j < 5; j++)
            cin >> p[i][j];
    }
    return p;
}
void Print(int **p)
{
    for(int i = 0; i < 4; i++)
    {
        for(int j = 0; j < 5; j++)
            cout << p[i][j] << " ";
        cout << endl;
    }
}
int ** Add(int **a, int **b)
{
    int **cur = new int *[4];
    for(int i = 0; i < 4; i++)
    {
        cur[i] = new int[5];
        for(int j = 0; j < 5; j++)
            cur[i][j] = a[i][j] + b[i][j];
    }
    return cur;
}
int ** Surplus(int **a, int **b)
{
    int **cur = new int *[4];
    for(int i = 0; i < 4; i++)
    {
        cur[i] = new int[5];
        for(int j = 0; j < 5; j++)
            cur[i][j] = a[i][j] - b[i][j];
    }
    return cur;
}
void del(int **a)
{
    for (int i = 0; i < 4; i++)
            delete[] a[i];
    delete[] a;
}
int main()
{
    cout << "请依次输入矩阵数据" << endl;
    int **A1 = Initialize();
    cout << "请依次输入矩阵数据" << endl;
    int **A2 = Initialize();
    int **A3;
    A3 = Add(A1,A2);
    cout << "相加结果如下：" << endl;
    Print(A3);
    A3 = Surplus(A1,A2);
    cout << "相减结果如下：" << endl;
    Print(A3);
    del(A1);
    del(A2);
    del(A3);
    return 0;
}